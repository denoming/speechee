#pragma once

#include <functional>
#include <string_view>
#include <system_error>

namespace jar {

class ISpeechSynthesizePool {
public:
    using OnCompleteSig = void(std::string audio, std::error_code error);

    virtual ~ISpeechSynthesizePool() = default;

    virtual void
    synthesizeText(std::string_view text,
                   std::string_view lang,
                   std::function<OnCompleteSig> callback)
        = 0;

    virtual void
    synthesizeSsml(std::string_view ssml,
                   std::string_view lang,
                   std::function<OnCompleteSig> callback)
        = 0;
};

} // namespace jar