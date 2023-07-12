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
    const auto [entry, ok] = _statuses.insert({subject.name(), subject.availability()});

    subject.onAvailabilityUpdate(sigc::track_obj(
        [entry, this](const std::string& name, const AvailabilityStatus newStatus) {
            std::get<AvailabilityStatus>(*entry) = newStatus;
            if (newStatus == AvailabilityStatus::Online) {
                updateAvailability();
            } else {
                availability(AvailabilityStatus::Offline);
            }
        },
        *this));

    updateAvailability();
}

void
AvailabilityObserver::remove(AvailabilitySubject& subject)
{
    if (_statuses.erase(subject.name()) > 0) {
        updateAvailability();
    }
}

void
AvailabilityObserver::updateAvailability()
{
    if (_statuses.empty()) {
        availability(AvailabilityStatus::Offline);
    } else {
        auto ownStatus = AvailabilityStatus::Online;
        for (const auto& [name, status] : _statuses) {
            if (status != AvailabilityStatus::Online) {
                ownStatus = AvailabilityStatus::Offline;
                break;
            }
        }
        availability(ownStatus);
    }

}

} // namespace jar