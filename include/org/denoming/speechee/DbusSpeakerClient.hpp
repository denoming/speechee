#pragma once

#include <org/denoming/speechee/ISpeakerClient.hpp>
#include <org/denoming/speechee/SpeecheeExport.hpp>

#include <memory>

namespace jar {

class SPEECHEE_EXPORT DbusSpeakerClient final : public ISpeakerClient {
public:
    DbusSpeakerClient();

    ~DbusSpeakerClient() final;

    void
    synthesizeText(const std::string& text, const std::string& lang) final;

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
