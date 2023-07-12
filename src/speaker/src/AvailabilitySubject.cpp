#include "speaker/AvailabilitySubject.hpp"

namespace jar {

AvailabilitySubject::AvailabilitySubject(std::string name)
    : _name{std::move(name)}
{
}

const std::string&
AvailabilitySubject::name() const
{
    return _name;
}

AvailabilityStatus
AvailabilitySubject::availability() const
{
    return _status;
}

sigc::connection
AvailabilitySubject::onAvailabilityUpdate(OnStateUpdateSignal::slot_type&& slot)
{
    return _signal.connect(std::move(slot));
}

void
AvailabilitySubject::availability(AvailabilityStatus status)
{
    if (_status != status) {
        _signal(_name, _status = status);
    }
}

} // namespace jar