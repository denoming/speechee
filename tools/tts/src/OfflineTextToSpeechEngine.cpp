#include "tts/OfflineTextToSpeechEngine.hpp"

#include <voxer/FormattedDataHandler.hpp>
#include <voxer/Voxer.hpp>

#include <gsl/gsl-lite.hpp>

#include <stdexcept>

namespace fs = std::filesystem;

namespace jar {

class AudioToText final : public FormattedDataHandler {
public:
    AudioToText()
        : FormattedDataHandler{DataFormat::Wav}
    {
    }

    void
    onFormattedBegin(DataFormat format, int sampleRate, int sampleWidth, int channels) override
    {
    }

    void
    onFormattedData(const void* ptr, size_t size) override
    {
        const char* typedPtr = static_cast<const char*>(ptr);
        _buffer.append(typedPtr, typedPtr + size);
    }

    void
    onFormattedEnd(const SynthesisResult& result) override
    {
    }

    [[nodiscard]] std::string
    str()
    {
        return std::move(_buffer);
    }

private:
    std::string _buffer;
};

class OfflineTextToSpeechEngine::Impl {
public:
    explicit Impl(const fs::path& modelPath)
    {
        if (not _voxer.configure(modelPath)) {
            throw std::runtime_error{"Unable to configure voxer"};
        }
    }

    Impl(const fs::path& modelPath, const fs::path& filesPath)
    {
        if (not _voxer.configure(modelPath, filesPath, false, SpeakerId::Default)) {
            throw std::runtime_error{"Unable to configure voxer"};
        }
    }

    std::string
    synthesizeText(const std::string& text, const Voice& voice, const AudioConfig& audioConfig)
    {
        AudioToText handler;
        std::ignore = _voxer.textToAudio(text, handler);
        return handler.str();
    }

private:
    Voxer _voxer;
};

OfflineTextToSpeechEngine::OfflineTextToSpeechEngine(const fs::path& modelPath)
    : _impl{std::make_unique<Impl>(modelPath)}
{
}

OfflineTextToSpeechEngine::OfflineTextToSpeechEngine(const std::filesystem::path& modelPath,
                                                     const std::filesystem::path& filesPath)
    : _impl{std::make_unique<Impl>(modelPath, filesPath)}
{
}

OfflineTextToSpeechEngine::~OfflineTextToSpeechEngine() = default;

std::string
OfflineTextToSpeechEngine::synthesizeText(const std::string& text,
                                          const Voice& voice,
                                          const AudioConfig& audioConfig)
{
    gsl_Assert(_impl);
    return _impl->synthesizeText(text, voice, audioConfig);
}

std::string
OfflineTextToSpeechEngine::synthesizeSsml(const std::string& ssml,
                                          const Voice& voice,
                                          const AudioConfig& audioConfig)
{
    throw std::logic_error{"Not supported"};
}

} // namespace jar