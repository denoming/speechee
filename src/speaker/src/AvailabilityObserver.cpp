#include "speaker/AvailabilityObserver.hpp"

#include <sigc++/adaptors/adaptors.h>

namespace jar {

AvailabilityObserver::AvailabilityObserver(std::string name)
    : AvailabilitySubject{std::move(name)}
{
}

void
AvailabilityObserver::add(AvailabilitySubject& subject)
{
    const auto [entry, ok] = _states.insert({subject.name(), subject.state()});

    subject.onStateUpdate(sigc::track_obj(
        [entry, this](const std::string& name, const AvailabilityState state) {
            std::get<AvailabilityState>(*entry) = state;
            if (state == AvailabilityState::Online) {
                updateAvailability();
            } else {
                availability(AvailabilityState::Offline);
            }
        },
        *this));

    updateAvailability();
}

void
AvailabilityObserver::remove(AvailabilitySubject& subject)
{
    if (_states.erase(subject.name()) > 0) {
        updateAvailability();
    }
}

void
AvailabilityObserver::updateAvailability()
{
    if (_states.empty()) {
        availability(AvailabilityState::Offline);
    } else {
        auto ownStatus = AvailabilityState::Online;
        for (const auto& [name, status] : _states) {
            if (status != AvailabilityState::Online) {
                ownStatus = AvailabilityState::Offline;
                break;
            }
        }
        availability(ownStatus);
    }
}

} // namespace jar