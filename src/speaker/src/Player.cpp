#include "speaker/Player.hpp"

#include "speaker/AudioBufferList.hpp"
#include "speaker/Formatters.hpp"
#include "speaker/PlayerLoop.hpp"

#include <jarvisto/Logger.hpp>

#include <boost/assert.hpp>

#include <gst/app/gstappsrc.h>
#include <gst/gst.h>
#include <gst/gstelement.h>

#include <atomic>

static constexpr guint64 kInputQueueMaxSize{1 * 1000 * 1000 /* 1 Mb */};

namespace jar {

class Player::Impl {
public:
    Impl() = default;

    [[nodiscard]] bool
    start()
    {
        if (state() != PlayState::Null) {
            LOGE("Inconsistent state of player");
            return false;
        }

        if (not startLoop()) {
            LOGE("Unable to start main loop");
            return false;
        }

        if (not createPipeline()) {
            stopLoop();
            LOGE("Unable to create pipeline");
            return false;
        }

        if (not startPipeline()) {
            destroyPipeline();
            stopLoop();
            LOGE("Unable to start pipeline");
            return false;
        }

        setState(PlayState::Idle);

        return true;
    }

    void
    stop()
    {
        if (not stopPipeline()) {
            LOGE("Unable to stop pipeline");
        }

        setState(PlayState::Null);

        destroyPipeline();
        stopLoop();
    }

    [[nodiscard]] PlayState
    state() const
    {
        return _state;
    }

    [[nodiscard]] bool
    play(const std::string_view audio)
    {
        if (state() == PlayState::Null or state() == PlayState::Error) {
            LOGE("Player is not ready to play");
            return false;
        }

        LOGD("Buffer <{}> bytes of audio content", audio.size());
        _bufferList.push(audio);

        setState(PlayState::Busy);

        if (not isFeedingActive()) {
            LOGD("Schedule pipeline feeding");
            scheduleFeeding();
        }

        return true;
    }

    OnStateUpdateSignal
    onStateUpdate()
    {
        return _onStateUpdateSig;
    }

private:
    void
    setState(PlayState newState)
    {
        if (const auto oldState = _state.load(); oldState != newState) {
            LOGD("Update state: {} -> {}", oldState, newState);
            _onStateUpdateSig(_state = newState);
        }
    }

    bool
    startLoop()
    {
        if (not _mainLoop.active()) {
            LOGD("Start main loop");
            _mainLoop.start();
        }
        return _mainLoop.active();
    }

    void
    stopLoop()
    {
        if (_mainLoop.active()) {
            LOGD("Stop main loop");
            _mainLoop.stop();
        }
    }

    void
    scheduleFeeding()
    {
        if (_feedingCon) {
            LOGW("Feeding is already scheduled");
            return;
        }
        _feedingCon = _mainLoop.onIdle(sigc::mem_fun(*this, &Impl::feedPipeline));
    }

    void
    cancelFeeding()
    {
        if (_feedingCon) {
            _feedingCon.disconnect();
        }
    }

    [[nodiscard]] bool
    isFeedingActive() const
    {
        return _feedingCon.connected();
    }

    [[nodiscard]] bool
    createPipeline()
    {
        if (not createBin()) {
            LOGE("Unable to create pipeline bin");
            return false;
        }

        if (not createBus()) {
            LOGE("Unable to create pipeline bus");
            return false;
        }

        if (not createElements()) {
            LOGE("Unable to create pipeline elements");
            return false;
        }

        return true;
    }

    void
    destroyPipeline()
    {
        destroyBus();
        destroyBin();
    }

    [[nodiscard]] bool
    createBin()
    {
        BOOST_ASSERT(_pipeline == nullptr);
        if (_pipeline = gst_pipeline_new("player"); _pipeline == nullptr) {
            LOGE("Unable to create pipeline");
            return false;
        }
        return true;
    }

    void
    destroyBin()
    {
        if (_pipeline != nullptr) {
            gst_object_unref(_pipeline);
            _pipeline = nullptr;
        }
    }

    [[nodiscard]] bool
    createBus()
    {
        BOOST_ASSERT(_pipeline != nullptr);
        auto* bus = gst_element_get_bus(_pipeline);
        if (bus == nullptr) {
            LOGE("Unable to get pipeline bus");
            return false;
        }
        gst_bus_add_watch(bus, GstBusFunc(onGstPipelineMessage), this);
        gst_object_unref(bus);
        return true;
    }

    void
    destroyBus() const
    {
        if (GstBus* bus = gst_element_get_bus(_pipeline); bus != nullptr) {
            if (not gst_bus_remove_watch(bus)) {
                LOGE("Unable to remove bus watcher");
            }
            gst_object_unref(bus);
        }
    }

    [[nodiscard]] bool
    createElements()
    {
        auto* const bin = GST_BIN(_pipeline);
        BOOST_ASSERT(bin != nullptr);

        auto* const src = gst_element_factory_make("appsrc", "src");
        BOOST_ASSERT(src != nullptr);
        if (not gst_bin_add(bin, src)) {
            LOGE("Unable to add element");
            return false;
        }
        g_signal_connect(src, "enough-data", G_CALLBACK(onGstPipelineEnoughData), this);
        g_signal_connect(src, "need-data", G_CALLBACK(onGstPipelineNeedData), this);
        g_object_set(src,
                     "is-live",
                     TRUE,
                     "max-bytes",
                     kInputQueueMaxSize,
                     "do-timestamp",
                     TRUE /* set timestamps for outgoing buffers */,
                     nullptr);

        auto* const parser = gst_element_factory_make("wavparse", nullptr);
        BOOST_ASSERT(parser != nullptr);
        if (not gst_bin_add(bin, parser)) {
            LOGE("Unable to add element");
            return false;
        }
        g_object_set(parser, "ignore-length", TRUE, nullptr);

        auto* const queue = gst_element_factory_make("queue", nullptr);
        BOOST_ASSERT(queue != nullptr);
        if (not gst_bin_add(bin, queue)) {
            LOGE("Unable to add element");
            return false;
        }

        auto* const converter = gst_element_factory_make("audioconvert", nullptr);
        BOOST_ASSERT(converter != nullptr);
        if (not gst_bin_add(bin, converter)) {
            LOGE("Unable to add element");
            return false;
        }

        auto* const resample = gst_element_factory_make("audioresample", nullptr);
        BOOST_ASSERT(resample != nullptr);
        if (not gst_bin_add(bin, resample)) {
            LOGE("Unable to add element");
            return false;
        }

        auto* const sink = gst_element_factory_make("autoaudiosink", nullptr);
        BOOST_ASSERT(sink != nullptr);
        if (not gst_bin_add(bin, sink)) {
            LOGE("Unable to add element");
            return false;
        }
        g_object_set(sink, "sync", FALSE, nullptr);

        if (gst_element_link_many(src, parser, converter, resample, queue, sink, nullptr) != TRUE) {
            LOGE("Unable to link pipeline elements");
            return false;
        }

        return true;
    }

    [[nodiscard]] bool
    startPipeline() const
    {
        BOOST_ASSERT(_pipeline != nullptr);
        return (gst_element_set_state(_pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE);
    }

    [[nodiscard]] bool
    stopPipeline() const
    {
        BOOST_ASSERT(_pipeline != nullptr);
        return (gst_element_set_state(_pipeline, GST_STATE_NULL) != GST_STATE_CHANGE_FAILURE);
    }

    [[nodiscard]] bool
    feedPipeline()
    {
        if (_bufferList.empty()) {
            LOGD("Cancel feeding, buffer is empty");
            setState(PlayState::Idle);
            return false;
        }

        GstElement* src = gst_bin_get_by_name(GST_BIN(_pipeline), "src");
        BOOST_ASSERT(src != nullptr);

        GstBuffer* buffer = _bufferList.pop();
        LOGD("Pushing buffer: size<{}>", gst_buffer_get_size(buffer));

        if (gst_app_src_push_buffer(GST_APP_SRC(src), buffer) != GST_FLOW_OK) {
            LOGE("Unable to push buffer");
        }

        return true;
    }

    void
    onError(const gchar* source, const gchar* message, gint code, const gchar* debugInfo)
    {
        LOGE("Error from <{}> element: {} <{}>", source, message, code);
        if (debugInfo != nullptr) {
            LOGE("Debugging info: {}", debugInfo);
        }

        if (not stopPipeline()) {
            LOGE("Unable to stop pipeline");
        }

        setState(PlayState::Error);
    }

    void
    onEos()
    {
        LOGD("EoS has been reached");

        if (not stopPipeline()) {
            LOGE("Unable to stop pipeline");
            setState(PlayState::Error);
        }

        setState(PlayState::Idle);
    }

    void
    onEnoughData(GstElement* /*appsrc*/)
    {
        LOGD("Pipeline enough data");

        cancelFeeding();
    }

    void
    onNeedData(GstElement* /*appsrc*/, guint /*length*/)
    {
        LOGD("Pipeline need data");

        if (not isFeedingActive()) {
            scheduleFeeding();
        }
    }

private:
    static gboolean
    onGstPipelineMessage(GstBus* /*bus*/, GstMessage* message, gpointer data)
    {
        auto* self = static_cast<Impl*>(data);
        if (self == nullptr) {
            LOGE("Invalid self pointer");
            return FALSE;
        }

        switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR: {
            gchar* debug{};
            GError* error{};
            gst_message_parse_error(message, &error, &debug);
            self->onError(GST_OBJECT_NAME(message->src), error->message, error->code, debug);
            g_clear_error(&error);
            g_free(debug);
            break;
        }
        case GST_MESSAGE_EOS: {
            self->onEos();
            break;
        }
        default:
            break;
        }

        return TRUE;
    }

    static void
    onGstPipelineEnoughData(GstElement* appsrc, gpointer data)
    {
        if (auto* self = static_cast<Impl*>(data); self == nullptr) {
            LOGE("Invalid self pointer");
        } else {
            self->onEnoughData(appsrc);
        }
    }

    static void
    onGstPipelineNeedData(GstElement* appsrc, const guint length, gpointer data)
    {
        if (auto* self = static_cast<Impl*>(data); self == nullptr) {
            LOGE("Invalid self pointer");
        } else {
            self->onNeedData(appsrc, length);
        }
    }

private:
    GstElement* _pipeline{};
    AudioBufferList _bufferList{kInputQueueMaxSize};
    PlayerLoop _mainLoop;
    std::atomic<PlayState> _state{PlayState::Null};
    sigc::connection _feedingCon;
    OnStateUpdateSignal _onStateUpdateSig;
};

Player::Player()
    : _impl{std::make_unique<Impl>()}
{
}

Player::~Player() = default;

PlayState
Player::state() const
{
    BOOST_ASSERT(_impl);
    return _impl->state();
}

bool
Player::start()
{
    BOOST_ASSERT(_impl);
    return _impl->start();
}

void
Player::stop()
{
    BOOST_ASSERT(_impl);
    _impl->stop();
}

bool
Player::play(std::string_view audio)
{
    BOOST_ASSERT(_impl);
    return _impl->play(audio);
}

sigc::connection
Player::onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->onStateUpdate().connect(std::move(slot));
}

} // namespace jar
