#pragma once

#include "speaker/Types.hpp"

#include <fmt/format.h>
#include <fmt/std.h>

template<>
struct fmt::formatter<jar::PlayState> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::PlayState state, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (state) {
        case jar::PlayState::Null:
            output = "Null";
            break;
        case jar::PlayState::Idle:
            output = "Idle";
            break;
        case jar::PlayState::Busy:
            output = "Busy";
            break;
        case jar::PlayState::Error:
            output = "Error";
            break;
        }
        return formatter<string_view>::format(output, context);
    }
};

template<>
struct fmt::formatter<jar::AvailabilityState> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::AvailabilityState state, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (state) {
        case jar::AvailabilityState::Online:
            output = "Online";
            break;
        case jar::AvailabilityState::Offline:
            output = "Offline";
            break;
        }
        return formatter<string_view>::format(output, context);
    }
};

template<>
struct fmt::formatter<jar::MqttConnectReturnCode> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::MqttConnectReturnCode code, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (code) {
        case jar::MqttConnectReturnCode::Accepted:
            output = "Accepted";
            break;
        case jar::MqttConnectReturnCode::UnacceptableProtocolVersion:
            output = "UnacceptableProtocolVersion";
            break;
        case jar::MqttConnectReturnCode::IdentifierRejected:
            output = "IdentifierRejected";
            break;
        case jar::MqttConnectReturnCode::ServerUnavailable:
            output = "ServerUnavailable";
            break;
        case jar::MqttConnectReturnCode::BadCredentials:
            output = "BadCredentials";
            break;
        case jar::MqttConnectReturnCode::NotAuthorized:
            output = "NotAuthorized";
            break;
        }
        return formatter<string_view>::format(output, context);
    }
};