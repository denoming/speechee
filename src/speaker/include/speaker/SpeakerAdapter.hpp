#pragma once

#include <string>

namespace sdbus {
class IObject;
}

namespace jar {

class SpeakerAdapter {
public:
    explicit SpeakerAdapter(sdbus::IObject& object);

    virtual ~SpeakerAdapter() = default;

    void
    registerAdaptor();

    virtual void
    synthesizeText(const std::string& text, const std::string& lang)
        = 0;

    virtual void
    synthesizeSsml(const std::string& ssml, const std::string& lang)
        = 0;

private:
    sdbus::IObject& _object;
};

} // namespace jar
