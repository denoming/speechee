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

#pragma once

#include "tts/ITextToSpeechClient.hpp"

#include <filesystem>
#include <memory>

namespace jar {

class OfflineTextToSpeechEngine final : public ITextToSpeechClient {
public:
    explicit OfflineTextToSpeechEngine(const std::filesystem::path& modelPath);

    OfflineTextToSpeechEngine(const std::filesystem::path& modelPath,
                              const std::filesystem::path& filesPath);

    ~OfflineTextToSpeechEngine() override;

    std::string
    synthesizeText(const std::string& text,
                   const Voice& voice,
                   const AudioConfig& audioConfig) override;

    std::string
    synthesizeSsml(const std::string& ssml,
                   const Voice& voice,
                   const AudioConfig& audioConfig) override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
