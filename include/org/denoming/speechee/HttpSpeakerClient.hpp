#pragma once

#include <org/denoming/speechee/ISpeakerClient.hpp>
#include <org/denoming/speechee/SpeecheeExport.hpp>

#include <memory>
#include <string>

namespace jar {

class SPEECHEE_EXPORT HttpSpeakerClient final : public ISpeakerClient {
public:
    static inline const char* kDefaultHost = "localhost";
    static inline const char* kDefaultPort = "2350";

    explicit HttpSpeakerClient(std::string host = kDefaultHost, std::string port = kDefaultPort);

    ~HttpSpeakerClient() final;

    void
    synthesizeText(const std::string& text, const std::string& lang) final;

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar