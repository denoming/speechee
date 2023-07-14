#pragma once

#include "speaker/Types.hpp"

#include <sigc++/signal.h>

#include <string>

namespace jar {

class IAvailabilitySubject {
public:
    using OnStateUpdate = void(const std::string& name, AvailabilityState state);
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IAvailabilitySubject() = default;

    [[nodiscard]] virtual const std::string&
    name() const
        = 0;

    [[nodiscard]] virtual AvailabilityState
    state() const
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
        = 0;
};

} // namespace jar