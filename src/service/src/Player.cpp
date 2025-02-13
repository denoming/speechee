#include "speechee/Player.hpp"

#include "speechee/AudioBufferList.hpp"
#include "speechee/Formatters.hpp"
#include "speechee/PlayerLoop.hpp"

#include <jarvisto/core/Logger.hpp>

#include <gsl/gsl-lite.hpp>

#include <gst/app/gstappsrc.h>
#include <gst/gst.h>
#include <gst/gstelement.h>

#include <atomic>

static constexpr guint64 kInputQueueMaxSize{1 * 1000 * 1000 /* 1 Mb */};

namespace jar {

class Player::Impl {
public:
    explicit Impl(IPlayerLoop& playerLoop)
        : _playerLoop{playerLoop}
    {}

    [[nodiscard]] bool
    start()
    {
        if (state() != PlayState::Null) {
            LOGE("Inconsistent state of player");
            return false;
        }

        if (not createPipeline()) {
            LOGE("Unable to create pipeline");
            return false;
        }

        if (not startPipeline()) {
            destroyPipeline();
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

    void
    scheduleFeeding()
    {
        if (_feedingCon) {
            LOGW("Feeding is already scheduled");
            return;
        }
        _feedingCon = _playerLoop.onIdle(sigc::mem_fun(*this, &Impl::feedPipeline));
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
        gsl_Assert(_pipeline == nullptr);
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
        gsl_Assert(_pipeline != nullptr);
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
        gsl_Assert(bin != nullptr);

        auto* const src = gst_element_factory_make("appsrc", "src");
        gsl_Assert(src != nullptr);
        if (not gst_bin_add(bin, src)) {
            LOGE("Unable to add element");
            return false;
        }
        g_object_set(src,
                     "is-live",
                     TRUE,
                     "max-bytes",
                     kInputQueueMaxSize,
                     "do-timestamp",
                     TRUE /* set timestamps for outgoing buffers */,
                     "block",
                     TRUE,
                     nullptr);

        auto* const parser = gst_element_factory_make("wavparse", nullptr);
        gsl_Assert(parser != nullptr);
        if (not gst_bin_add(bin, parser)) {
            LOGE("Unable to add element");
            return false;
        }
        // g_object_set(parser, "ignore-length", TRUE, nullptr);

        auto* const queue = gst_element_factory_make("queue", nullptr);
        gsl_Assert(queue != nullptr);
        if (not gst_bin_add(bin, queue)) {
            LOGE("Unable to add element");
            return false;
        }

        auto* const converter = gst_element_factory_make("audioconvert", nullptr);
        gsl_Assert(converter != nullptr);
        if (not gst_bin_add(bin, converter)) {
            LOGE("Unable to add element");
            return false;
        }

        auto* const resample = gst_element_factory_make("audioresample", nullptr);
        gsl_Assert(resample != nullptr);
        if (not gst_bin_add(bin, resample)) {
            LOGE("Unable to add element");
            return false;
        }

        auto* const sink = gst_element_factory_make("autoaudiosink", nullptr);
        gsl_Assert(sink != nullptr);
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
        gsl_Assert(_pipeline != nullptr);
        return (gst_element_set_state(_pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE);
    }

    [[nodiscard]] bool
    stopPipeline() const
    {
        gsl_Assert(_pipeline != nullptr);
        return (gst_element_set_state(_pipeline, GST_STATE_NULL) != GST_STATE_CHANGE_FAILURE);
    }

    [[nodiscard]] bool
    feedPipeline()
    {
        GstElement* src = gst_bin_get_by_name(GST_BIN(_pipeline), "src");
        gsl_Assert(src != nullptr);

        if (_bufferList.empty()) {
            LOGD("Cancel feeding, buffer is empty");
            gst_app_src_end_of_stream(GST_APP_SRC(src));
            return false;
        }

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
        LOGD("EoS has been reached: {}", std::this_thread::get_id());

        if (not stopPipeline()) {
            LOGE("Unable to stop pipeline");
            setState(PlayState::Error);
        }

        setState(PlayState::Idle);
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

private:
    IPlayerLoop& _playerLoop;
    GstElement* _pipeline{};
    AudioBufferList _bufferList{kInputQueueMaxSize};
    std::atomic<PlayState> _state{PlayState::Null};
    sigc::connection _feedingCon;
    OnStateUpdateSignal _onStateUpdateSig;
};

Player::Player(IPlayerLoop& playerLoop)
    : _impl{std::make_unique<Impl>(playerLoop)}
{
}

Player::~Player() = default;

PlayState
Player::state() const
{
    gsl_Assert(_impl);
    return _impl->state();
}

bool
Player::start()
{
    gsl_Assert(_impl);
    return _impl->start();
}

void
Player::stop()
{
    gsl_Assert(_impl);
    _impl->stop();
}

bool
Player::play(std::string_view audio)
{
    gsl_Assert(_impl);
    return _impl->play(audio);
}

[[nodiscard]] Player::OnStateUpdateSignal
Player::onStateUpdate()
{
    gsl_Assert(_impl);
    return _impl->onStateUpdate();
}

} // namespace jar
