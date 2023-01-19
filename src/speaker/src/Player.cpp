#include "speaker/Player.hpp"

#include "jarvis/Logger.hpp"
#include "speaker/AudioBufferList.hpp"
#include "speaker/Formatters.hpp"
#include "speaker/PlayerLoop.hpp"

#include <boost/assert.hpp>
#include <gst/gst.h>
#include <gst/gstelement.h>

#include <atomic>

namespace {

bool
initializeGst()
{
    GError* error{nullptr};
    if (gst_init_check(NULL, NULL, &error) != TRUE) {
        BOOST_ASSERT(error != nullptr);
        LOGE("Initialization failed: code: <{}>, domain: <{}>, message: <{}>",
             error->code,
             error->domain,
             error->message);
        return false;
    }
    return true;
}

void
finalizeGst()
{
    if (gst_is_initialized() == TRUE) {
        gst_deinit();
    }
}

} // namespace

namespace jar {

class Player::Impl {
public:
    Impl()
        : _pipeline{nullptr}
        , _dataRequired{false}
        , _state{PlayState::Null}
    {
    }

    bool
    initialize()
    {
        LOGI("Initialize player");

        if (state() != PlayState::Null) {
            LOGE("Inconsistent state of player");
            return false;
        }

        LOGD("Initialize gstreamer");
        if (!initializeGst()) {
            LOGE("Failed to initialize gstreamer");
            finalize();
            return false;
        }

        LOGD("Create gstreamer pipeline");
        if (!createPipeline()) {
            LOGE("Failed to create pipeline");
            finalize();
            return false;
        }

        LOGD("Start pipeline main loop");
        startLoop();

        setState(PlayState::Idle);

        return true;
    }

    void
    finalize()
    {
        LOGI("Finalize player");

        setState(PlayState::Null);

        LOGD("Stop pipeline main loop");
        stopLoop();
        LOGD("Destroy gstreamer pipeline");
        destroyPipeline();
        LOGD("Finalize gstreamer");
        finalizeGst();
    }

    PlayState
    state() const
    {
        return _state;
    }

    bool
    play(std::string_view audio)
    {
        LOGI("Play audio content: size<{}>", audio.size());

        if (state() != PlayState::Idle) {
            LOGE("Inconsistent state of player");
            return false;
        }

        LOGD("Start pipeline");
        if (!startPipeline()) {
            LOGE("Failed to apply playing state towards player pipeline");
            setState(PlayState::Error);
            return false;
        }

        BOOST_ASSERT(!_bufferList);
        _bufferList = std::make_unique<AudioBufferList>(audio);

        setState(PlayState::Busy);

        BOOST_ASSERT(_mainLoop);
        _mainLoop->invoke([this]() { return feedPipeline(); });

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
        const auto oldState = _state.load();
        if (oldState == newState) {
            return;
        }
        LOGI("Update state: {} -> {}", oldState, newState);
        _onStateUpdateSig(_state = newState);
    }

    void
    startLoop()
    {
        BOOST_ASSERT(!_mainLoop);
        _mainLoop = std::make_unique<PlayerLoop>();
    }

    void
    stopLoop()
    {
        _mainLoop.reset();
    }

    bool
    createPipeline()
    {
        auto* src = gst_element_factory_make("appsrc", "src");
        auto* parser = gst_element_factory_make("wavparse", nullptr);
        auto* converter = gst_element_factory_make("audioconvert", nullptr);
        auto* sink = gst_element_factory_make("autoaudiosink", nullptr);
        if (!src || !parser || !converter || !sink) {
            LOGE("Failed to create pipeline elements");
            return false;
        }

        if (_pipeline = gst_pipeline_new("player"); _pipeline != nullptr) {
            gst_bin_add_many(GST_BIN(_pipeline), src, parser, converter, sink, NULL);
        } else {
            LOGE("Failed to create pipeline");
            return false;
        }

        if (gst_element_link_many(src, parser, converter, sink, NULL) != TRUE) {
            finalize();
            LOGE("Failed to link pipeline elements");
            return false;
        }

        g_signal_connect(src, "enough-data", G_CALLBACK(onPipelineEnoughDataStatic), this);
        g_signal_connect(src, "need-data", G_CALLBACK(onPipelineNeedDataStatic), this);

        auto* bus = gst_element_get_bus(_pipeline);
        BOOST_ASSERT(bus != nullptr);
        gst_bus_add_watch(bus, GstBusFunc(onPipelineMessageStatic), this);
        gst_object_unref(bus);

        return true;
    }

    void
    destroyPipeline()
    {
        if (!_pipeline) {
            return;
        }

        auto* bus = gst_element_get_bus(_pipeline);
        BOOST_ASSERT(bus != nullptr);
        gst_bus_remove_watch(bus);

        gst_element_set_state(_pipeline, GST_STATE_NULL);
        gst_object_unref(_pipeline);
        _pipeline = nullptr;
    }

    bool
    startPipeline()
    {
        BOOST_ASSERT(_pipeline != nullptr);
        return (gst_element_set_state(_pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE);
    }

    bool
    stopPipeline()
    {
        BOOST_ASSERT(_pipeline != nullptr);
        return (gst_element_set_state(_pipeline, GST_STATE_NULL) != GST_STATE_CHANGE_FAILURE);
    }

    bool
    feedPipeline()
    {
        if (!_dataRequired) {
            return true;
        }

        if (state() != PlayState::Busy) {
            return false;
        }

        auto* src = gst_bin_get_by_name(GST_BIN(_pipeline), "src");
        BOOST_ASSERT(src != nullptr);
        if (_bufferList) {
            if (_bufferList->empty()) {
                LOGD("Buffer is empty");
                return false;
            } else {
                auto* buffer = _bufferList->pop();
                GstFlowReturn ret{GST_FLOW_OK};
                g_signal_emit_by_name(src, "push-buffer", buffer, &ret);
                gst_buffer_unref(buffer);
                if (ret != GstFlowReturn::GST_FLOW_OK) {
                    LOGE("Failed to push buffer");
                }
            }
        } else {
            LOGD("Buffer is invalid");
            return false;
        }

        return true;
    }

    void
    onPipelineError(const gchar* source, const gchar* message, gint code, const gchar* debugInfo)
    {
        LOGE("Error from <{}> element: {} <{}>", source, message, code);
        if (debugInfo != nullptr) {
            LOGE("Debugging info: {}", debugInfo);
        }

        _bufferList.reset();

        if (!stopPipeline()) {
            LOGE("Failed to stop pipeline");
        }

        setState(PlayState::Error);
    }

    void
    onPipelineEos()
    {
        LOGD("EoS has been reached");

        _bufferList.reset();

        if (!stopPipeline()) {
            LOGE("Failed to stop pipeline");
            setState(PlayState::Error);
        }

        setState(PlayState::Idle);
    }

    void
    onPipelineStateUpdate(GstObject* source, GstState oldState, GstState newState)
    {
        if (source == GST_OBJECT(_pipeline)) {
            LOGD("Pipeline state update: {} -> {}",
                 gst_element_state_get_name(oldState),
                 gst_element_state_get_name(newState));
        }
    }

    void
    onPipelineEnoughData(GstElement* /*appsrc*/)
    {
        _dataRequired = false;
    }

    void
    onPipelineNeedData(GstElement* /*appsrc*/, guint /*length*/)
    {
        _dataRequired = true;
    }

private:
    static gboolean
    onPipelineMessageStatic(GstBus* /*bus*/, GstMessage* message, gpointer data)
    {
        auto* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);

        if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR) {
            gchar* debug{nullptr};
            GError* error{nullptr};
            gst_message_parse_error(message, &error, &debug);
            const auto* source = GST_OBJECT_NAME(message->src);
            self->onPipelineError(source, error->message, error->code, debug);
            g_clear_error(&error);
            g_free(debug);
            return TRUE;
        }

        if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_STATE_CHANGED) {
            GstState oldState, newState, pendingState;
            gst_message_parse_state_changed(message, &oldState, &newState, &pendingState);
            self->onPipelineStateUpdate(GST_MESSAGE_SRC(message), oldState, newState);
            return TRUE;
        }

        if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_EOS) {
            self->onPipelineEos();
            return TRUE;
        }

        return TRUE;
    }

    static void
    onPipelineEnoughDataStatic(GstElement* appsrc, gpointer data)
    {
        auto* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onPipelineEnoughData(appsrc);
    }

    static void
    onPipelineNeedDataStatic(GstElement* appsrc, guint length, gpointer data)
    {
        auto* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onPipelineNeedData(appsrc, length);
    }

private:
    GstElement* _pipeline;
    AudioBufferList::Ptr _bufferList;
    PlayerLoop::Ptr _mainLoop;
    std::atomic<bool> _dataRequired;
    std::atomic<PlayState> _state;
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
    return _impl->state();
}

bool
Player::initialize()
{
    return _impl->initialize();
}

void
Player::finalize()
{
    _impl->finalize();
}

bool
Player::play(std::string_view audio)
{
    return _impl->play(audio);
}

sigc::connection
Player::onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
{
    return _impl->onStateUpdate().connect(std::move(slot));
}

} // namespace jar
