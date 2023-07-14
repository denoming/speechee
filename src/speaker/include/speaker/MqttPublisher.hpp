#pragma once

#include "speaker/Types.hpp"

#include <sigc++/signal.h>

#include <memory>
#include <string_view>

namespace jar {

class MqttPublisher {
public:
    /* Signatures */
    using OnConnect = void(bool success, MqttConnectReturnCode code);
    using OnDisconnect = void(bool success);
    /* Signals */
    using OnConnectSignal = sigc::signal<OnConnect>;
    using OnDisconnectSignal = sigc::signal<OnDisconnect>;

    explicit MqttPublisher(const char* id = nullptr, bool cleanSession = true);

    ~MqttPublisher();

    [[nodiscard]] bool
    connected() const;

    [[maybe_unused]] bool
    credentials(std::string_view user, std::string_view password);

    [[maybe_unused]] bool
    connect(std::string_view host, std::uint16_t port = 1883, int keepAlive = 60);

    [[maybe_unused]] bool
    disconnect(bool force = false);

    [[maybe_unused]] bool
    publish(std::string_view topic, std::string_view data, int qos = 2, bool retain = false);

    [[maybe_unused]] bool
    publish(std::string_view topic, void* data, std::size_t size, int qos = 2, bool retain = false);

    [[maybe_unused]] sigc::connection
    onConnect(OnConnectSignal::slot_type&& slot);

    [[maybe_unused]] sigc::connection
    onDisconnect(OnDisconnectSignal::slot_type&& slot);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar