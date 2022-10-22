#include "speech/TextToSpeechClient.hpp"

#include "jarvis/Logger.hpp"

#include <google/cloud/common_options.h>
#include <google/cloud/grpc_options.h>
#include <google/cloud/texttospeech/text_to_speech_client.h>
#include <google/cloud/texttospeech/text_to_speech_options.h>

#include <boost/assert.hpp>

namespace cloud = google::cloud;
namespace tts = cloud::texttospeech;

cloud::Options
getConnectionOptions(bool enableTracing = false)
{
    cloud::Options opts;

    if (enableTracing) {
        opts.lookup<cloud::TracingComponentsOption>().insert("rpc");
        opts.lookup<cloud::TracingComponentsOption>().insert("rpc-streams");
    }

    opts.set<cloud::GrpcCredentialOption>(grpc::GoogleDefaultCredentials());
    opts.set<cloud::GrpcNumChannelsOption>(128);
    opts.set<cloud::GrpcBackgroundThreadPoolSizeOption>(4);

    return opts;
}

namespace jar {

class TextToSpeechClient::Impl {
public:
    Impl()
        : _connection{tts::MakeTextToSpeechConnection(getConnectionOptions())}
    {
    }

    std::string
    synthesizeText(const std::string& text, const std::string& languageCode, std::error_code& error)
    {
        tts::v1::SynthesisInput input;
        input.set_text(text);
        return doSynthesize(input, languageCode, error);
    }

    std::string
    synthesizeSsml(const std::string& ssml, const std::string& languageCode, std::error_code& error)
    {
        tts::v1::SynthesisInput input;
        input.set_ssml(ssml);
        return doSynthesize(input, languageCode, error);
    }

private:
    std::string
    doSynthesize(const tts::v1::SynthesisInput& input,
                 const std::string& languageCode,
                 std::error_code& error)
    {
        tts::v1::VoiceSelectionParams voice;
        voice.set_language_code(languageCode);
        tts::v1::AudioConfig audio;
        audio.set_audio_encoding(tts::v1::LINEAR16);

        tts::TextToSpeechClient client{_connection};
        auto response = client.SynthesizeSpeech(input, voice, audio);
        if (!response) {
            LOGE("Text synthesize has failed: {}", response.status().message());
            error = std::make_error_code(std::errc::protocol_error);
            return {};
        }
        return std::move(*response->release_audio_content());
    }

private:
    std::shared_ptr<tts::TextToSpeechConnection> _connection;
};

TextToSpeechClient::TextToSpeechClient()
    : _impl{std::make_unique<Impl>()}
{
}

TextToSpeechClient::~TextToSpeechClient() = default;

std::string
TextToSpeechClient::synthesizeText(const std::string& text,
                                   const std::string& lang,
                                   std::error_code& error)
{
    BOOST_ASSERT(_impl);
    return _impl->synthesizeText(text, lang, error);
}

std::string
TextToSpeechClient::synthesizeSsml(const std::string& ssml,
                                   const std::string& lang,
                                   std::error_code& error)
{
    BOOST_ASSERT(_impl);
    return _impl->synthesizeSsml(ssml, lang, error);
}

} // namespace jar