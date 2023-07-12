#pragma once

#include "speaker/IAvailabilitySubject.hpp"

namespace jar {

class AvailabilitySubject : public IAvailabilitySubject {
public:
    using OnStateUpdate = void(const std::string& name, AvailabilityStatus state);
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    explicit AvailabilitySubject(std::string name);

    [[nodiscard]] const std::string&
    name() const override;

    [[nodiscard]] AvailabilityStatus
    availability() const override;

    [[maybe_unused]] sigc::connection
    onAvailabilityUpdate(OnStateUpdateSignal::slot_type&& slot) override;

protected:
    void
    availability(AvailabilityStatus status);

private:
    std::string _name;
    AvailabilityStatus _status{AvailabilityStatus::Offline};
    OnStateUpdateSignal _signal;
};

} // namespace jar