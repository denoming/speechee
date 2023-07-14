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

AvailabilityState
AvailabilitySubject::state() const
{
    return _state;
}

sigc::connection
AvailabilitySubject::onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
{
    return _signal.connect(std::move(slot));
}

void
AvailabilitySubject::availability(AvailabilityState state)
{
    if (_state != state) {
        _signal(_name, _state = state);
    }
}

} // namespace jar