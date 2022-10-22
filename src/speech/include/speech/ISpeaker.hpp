#pragma once

#include <memory>
#include <string_view>

namespace jar {

class ISpeaker {
public:
    using Ptr = std::unique_ptr<ISpeaker>;

    virtual ~ISpeaker() = default;

    virtual void
    synthesizeText(std::string_view text, std::string_view lang)
        = 0;

    virtual void
    synthesizeSsml(std::string_view ssml, std::string_view lang)
        = 0;
};

} // namespace jar