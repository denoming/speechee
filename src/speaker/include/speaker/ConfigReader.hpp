#pragma once

#include "jarvisto/Export.hpp"

#include <filesystem>
#include <string_view>

#include <libconfig.h++>

namespace jar {

class JARVISTO_EXPORT ConfigReader {
public:
    ConfigReader() = default;

    virtual ~ConfigReader() = default;

    [[nodiscard]] bool
    readString(std::string_view str);

    [[nodiscard]] bool
    readFile(const std::filesystem::path& path);

protected:
    virtual bool
    doParse(const libconfig::Config& config)
        = 0;
};

} // namespace jar