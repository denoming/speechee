#pragma once

#include "speechee/ISpeakerClient.hpp"
#include "speechee/SpeecheeExport.hpp"

#include <memory>

namespace jar {

class SPEECHEE_EXPORT DbusSpeakerClient final : public ISpeakerClient {
public:
    DbusSpeakerClient();

    ~DbusSpeakerClient() override;

    void
    synthesizeText(const std::string& text, const std::string& lang) override;

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang) override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
