#include "speaker/MqttPublisher.hpp"

#include <boost/assert.hpp>
#include <mosquitto.h>

#include <atomic>

namespace jar {

class MqttPublisher::Impl final {
public:
    explicit Impl(const char* id = nullptr, bool cleanSession = true)
    {
        mosquitto_lib_init();

        _handle = mosquitto_new(id, cleanSession, this);

        mosquitto_connect_callback_set(_handle, &onMosquittoConnect);
        mosquitto_disconnect_callback_set(_handle, &onMosquittoDisconnect);
    }

    ~Impl()
    {
        if (_handle != nullptr) {
            mosquitto_destroy(_handle);
        }

        mosquitto_lib_cleanup();
    }

    [[nodiscard]] bool
    connected() const
    {
        return _connected;
    }

    [[nodiscard]] bool
    credentials(std::string_view user, std::string_view password)
    {
        return (mosquitto_username_pw_set(_handle, user.data(), password.data())
                == MOSQ_ERR_SUCCESS);
    }

    bool
    connect(std::string_view host, std::uint16_t port, int keepAlive)
    {
        if (mosquitto_loop_start(_handle) == MOSQ_ERR_SUCCESS) {
            return (mosquitto_connect_async(_handle, host.data(), port, keepAlive)
                    == MOSQ_ERR_SUCCESS);
        }
        return false;
    }

    bool
    disconnect(bool force)
    {
        bool disconnected = (mosquitto_disconnect(_handle) == MOSQ_ERR_SUCCESS);
        mosquitto_loop_stop(_handle, force);
        return disconnected;
    }

    bool
    publish(std::string_view topic, const void* data, std::size_t size, int qos, bool retain)
    {
        return (mosquitto_publish(
                    _handle, nullptr, topic.data(), static_cast<int>(size), data, qos, retain)
                == MOSQ_ERR_SUCCESS);
    }

    sigc::connection
    onConnect(OnConnectSignal::slot_type&& slot)
    {
        return _onConnectSig.connect(std::move(slot));
    }

    sigc::connection
    onDisconnect(OnDisconnectSignal::slot_type&& slot)
    {
        return _onDisconnectSig.connect(std::move(slot));
    }

private:
    void
    connected(bool status)
    {
        _connected = status;
    }

    void
    notifyConnect(ConnectReturnCode returnCode)
    {
        _onConnectSig(returnCode);
    }

    void
    notifyDisconnect(bool expected)
    {
        _onDisconnectSig(expected);
    }

    static void
    onMosquittoConnect(mosquitto* /*mosq*/, void* object, int code)
    {
        Impl* self = static_cast<Impl*>(object);
        BOOST_ASSERT(self != nullptr);
        self->connected(code == int(ConnectReturnCode::Accepted));
        self->notifyConnect(ConnectReturnCode{code});
    }

    static void
    onMosquittoDisconnect(mosquitto* /*mosq*/, void* object, int code)
    {
        Impl* self = static_cast<Impl*>(object);
        BOOST_ASSERT(self != nullptr);
        self->connected(false);
        self->notifyDisconnect(code == 0);
    }

private:
    std::atomic<bool> _connected{false};
    OnConnectSignal _onConnectSig;
    OnDisconnectSignal _onDisconnectSig;
    mosquitto* _handle{nullptr};
};

MqttPublisher::MqttPublisher(const char* id, bool cleanSession)
    : _impl{std::make_unique<Impl>(id, cleanSession)}
{
}

MqttPublisher::~MqttPublisher() = default;

bool
MqttPublisher::connected() const
{
    BOOST_ASSERT(_impl);
    return _impl->connected();
}

bool
MqttPublisher::credentials(std::string_view user, std::string_view password)
{
    BOOST_ASSERT(_impl);
    return _impl->credentials(user, password);
}

bool
MqttPublisher::connect(std::string_view host, std::uint16_t port, int keepAlive)
{
    BOOST_ASSERT(_impl);
    return _impl->connect(host, port, keepAlive);
}

bool
MqttPublisher::disconnect(bool force)
{
    BOOST_ASSERT(_impl);
    return _impl->disconnect(force);
}

bool
MqttPublisher::publish(std::string_view topic, std::string_view data, int qos, bool retain)
{
    BOOST_ASSERT(_impl);
    return _impl->publish(topic, data.data(), data.size(), qos, retain);
}

bool
MqttPublisher::publish(std::string_view topic, void* data, std::size_t size, int qos, bool retain)
{
    BOOST_ASSERT(_impl);
    return _impl->publish(topic, data, size, qos, retain);
}

sigc::connection
MqttPublisher::onConnect(OnConnectSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->onConnect(std::move(slot));
}

sigc::connection
MqttPublisher::onDisconnect(OnDisconnectSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->onDisconnect(std::move(slot));
}

} // namespace jar