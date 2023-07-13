#pragma once

#include <sigc++/signal.h>

#include <memory>
#include <string_view>

namespace jar {

class MqttPublisher {
public:
    enum class ConnectReturnCode {
        Accepted = 0,
        UnacceptableProtocolVersion,
        IdentifierRejected,
        ServerUnavailable,
        BadCredentials,
        NotAuthorized
    };

    /* Signatures */
    using OnConnect = void(ConnectReturnCode);
    using OnDisconnect = void(bool unexpected);
    /* Signals */
    using OnConnectSignal = sigc::signal<OnConnect>;
    using OnDisconnectSignal = sigc::signal<OnDisconnect>;

    explicit MqttPublisher(const char* id = nullptr, bool cleanSession = true);

    ~MqttPublisher();

    [[nodiscard]] bool
    connected() const;

    [[nodiscard]] bool
    credentials(std::string_view user, std::string_view password);

    [[nodiscard]] bool
    connect(std::string_view host, std::uint16_t port = 1883, int keepAlive = 60);

    [[nodiscard]] bool
    disconnect(bool force = false);

    [[nodiscard]] bool
    publish(std::string_view topic, std::string_view data, int qos = 2, bool retain = false);

    [[nodiscard]] bool
    publish(std::string_view topic, void* data, std::size_t size, int qos = 2, bool retain = false);

    [[nodiscard]] sigc::connection
    onConnect(OnConnectSignal::slot_type&& slot);

    [[nodiscard]] sigc::connection
    onDisconnect(OnDisconnectSignal::slot_type&& slot);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar