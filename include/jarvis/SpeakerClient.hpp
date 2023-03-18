#pragma once

#include <memory>
#include <string>

#include "jarvis/SpeakerExport.hpp"

namespace jar {

class JARVIS_SPEAKER_EXPORT SpeakerClient {
public:
    SpeakerClient();

    virtual ~SpeakerClient();

    void
    synthesizeText(const std::string& text, const std::string& lang);

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
