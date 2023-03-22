#pragma once

#include <string>

namespace jar {

class ISpeakerClient {
public:
    virtual ~ISpeakerClient() = default;

    virtual void
    synthesizeText(const std::string& text, const std::string& lang)
        = 0;

    virtual void
    synthesizeSsml(const std::string& ssml, const std::string& lang)
        = 0;
};

} // namespace jar