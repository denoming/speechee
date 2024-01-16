#pragma once

#include <functional>
#include <string_view>
#include <system_error>

namespace jar {

class ISpeechSynthesizePool {
public:
    using OnDone = void(std::string audio, std::exception_ptr exception);

    virtual ~ISpeechSynthesizePool() = default;

    virtual void
    synthesizeText(std::string_view text,
                   std::string_view lang,
                   std::move_only_function<OnDone> callback)
        = 0;

    virtual void
    synthesizeSsml(std::string_view ssml,
                   std::string_view lang,
                   std::move_only_function<OnDone> callback)
        = 0;
};

} // namespace jar
