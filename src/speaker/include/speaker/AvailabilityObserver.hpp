#pragma once

#include "speaker/AvailabilitySubject.hpp"
#include "speaker/Types.hpp"

#include <sigc++/trackable.h>

#include <string>
#include <unordered_map>

namespace jar {

class AvailabilityObserver : public AvailabilitySubject, public sigc::trackable {
public:
    explicit AvailabilityObserver(std::string name);

    void
    add(AvailabilitySubject& subject);

    void
    remove(AvailabilitySubject& subject);

private:
    void
    updateAvailability();

private:
    std::unordered_map<std::string, AvailabilityState> _states;
};

} // namespace jar