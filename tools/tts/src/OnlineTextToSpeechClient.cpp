// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "tts/OnlineTextToSpeechClient.hpp"

#include <google/cloud/common_options.h>
#include <google/cloud/grpc_options.h>
#include <google/cloud/texttospeech/text_to_speech_client.h>
#include <google/cloud/texttospeech/text_to_speech_options.h>

#include <boost/assert.hpp>

namespace cloud = google::cloud;
namespace tts = cloud::texttospeech;

namespace jar {

namespace {

cloud::Options
getConnectionOptions()
{
    cloud::Options opts;

    opts.set<cloud::GrpcCredentialOption>(grpc::GoogleDefaultCredentials());
    opts.set<cloud::GrpcNumChannelsOption>(128);
    opts.set<cloud::GrpcBackgroundThreadPoolSizeOption>(4);

    return opts;
}

tts::v1::SsmlVoiceGender
toSsmlVoiceGender(SsmlVoiceGender gender)
{
    switch (gender) {
        using enum SsmlVoiceGender;
    case Male:
        return tts::v1::SsmlVoiceGender::MALE;
    case Female:
        return tts::v1::SsmlVoiceGender::FEMALE;
    default:
        return tts::v1::SsmlVoiceGender::SSML_VOICE_GENDER_UNSPECIFIED;
    }
}

tts::v1::AudioEncoding
toAudioEncoding(AudioEncoding encoding)
{
    switch (encoding) {
        using enum AudioEncoding;
    case MP3:
        return tts::v1::AudioEncoding::MP3;
    case OggOpus:
        return tts::v1::AudioEncoding::OGG_OPUS;
    case Mulaw:
        return tts::v1::AudioEncoding::MULAW;
    case Alaw:
        return tts::v1::AudioEncoding::ALAW;
    default:
        return tts::v1::AudioEncoding::LINEAR16;
    }
}

} // namespace

class OnlineTextToSpeechClient::Impl final : public ITextToSpeechClient {
public:
    Impl()
        : _connection{tts::MakeTextToSpeechConnection(getConnectionOptions())}
    {
    }

    std::string
    synthesizeText(const std::string& text,
                   const Voice& voice,
                   const AudioConfig& audioConfig) final
    {
        tts::v1::SynthesisInput input;
        input.set_text(text);
        return doSynthesize(input, voice, audioConfig);
    }

    std::string
    synthesizeSsml(const std::string& ssml,
                   const Voice& voice,
                   const AudioConfig& audioConfig) final
    {
        tts::v1::SynthesisInput input;
        input.set_ssml(ssml);
        return doSynthesize(input, voice, audioConfig);
    }

private:
    std::string
    doSynthesize(const tts::v1::SynthesisInput& input,
                 const Voice& voice,
                 const AudioConfig& audioConfig)
    {
        tts::v1::VoiceSelectionParams inVoice;
        inVoice.set_language_code(voice.languageCode);
        inVoice.set_name(voice.name);
        inVoice.set_ssml_gender(toSsmlVoiceGender(voice.ssmlGender));

        tts::v1::AudioConfig inAudioConfig;
        inAudioConfig.set_audio_encoding(toAudioEncoding(audioConfig.encoding));
        inAudioConfig.set_speaking_rate(audioConfig.speakingRate);
        inAudioConfig.set_pitch(audioConfig.pitch);
        inAudioConfig.set_volume_gain_db(audioConfig.volumeGainDb);
        inAudioConfig.set_sample_rate_hertz(audioConfig.sampleRateHz);
        std::for_each(
            std::begin(audioConfig.effects),
            std::end(audioConfig.effects),
            [&](const std::string& effect) { inAudioConfig.add_effects_profile_id(effect); });

        tts::TextToSpeechClient client{_connection};
        auto response = client.SynthesizeSpeech(input, inVoice, inAudioConfig);
        if (not response) {
            throw cloud::RuntimeStatusError{response.status()};
        }
        return std::move(*response->release_audio_content());
    }

private:
    std::shared_ptr<tts::TextToSpeechConnection> _connection;
};

OnlineTextToSpeechClient::OnlineTextToSpeechClient()
    : _impl{std::make_unique<Impl>()}
{
}

OnlineTextToSpeechClient::~OnlineTextToSpeechClient() = default;

std::string
OnlineTextToSpeechClient::synthesizeText(const std::string& text)
{
    return synthesizeText(text, Voice{}, AudioConfig{});
}

std::string
OnlineTextToSpeechClient::synthesizeText(const std::string& text,
                                   const Voice& voice,
                                   const AudioConfig& audioConfig)
{
    BOOST_ASSERT(_impl);
    return _impl->synthesizeText(text, voice, audioConfig);
}

std::string
OnlineTextToSpeechClient::synthesizeSsml(const std::string& ssml)
{
    return synthesizeSsml(ssml, Voice{}, AudioConfig{});
}

std::string
OnlineTextToSpeechClient::synthesizeSsml(const std::string& ssml,
                                   const Voice& voice,
                                   const AudioConfig& audioConfig)
{
    BOOST_ASSERT(_impl);
    return _impl->synthesizeSsml(ssml, voice, audioConfig);
}

} // namespace jar
