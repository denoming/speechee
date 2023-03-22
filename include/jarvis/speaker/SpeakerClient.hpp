#pragma once

#include <memory>

#include "jarvis/speaker/ISpeakerClient.hpp"
#include "jarvis/speaker/SpeakerExport.hpp"

namespace jar {

class JARVIS_SPEAKER_EXPORT SpeakerClient final : public ISpeakerClient {
public:
    SpeakerClient();

    ~SpeakerClient() final;

    void
    synthesizeText(const std::string& text, const std::string& lang) final;

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
