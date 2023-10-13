#include "speaker/ConfigReader.hpp"

#include <jarvisto/Logger.hpp>

namespace fs = std::filesystem;

namespace jar {

bool
ConfigReader::readString(std::string_view str)
{
    try {
        libconfig::Config cfg;
        cfg.readString(str.data());
        return doParse(cfg);
    } catch (const libconfig::ParseException& e) {
        LOGE("Unable to parse string on <{}> line: {}", e.getLine(), e.getError());
    }
    return false;
}

bool
ConfigReader::readFile(const std::filesystem::path& file)
{
    try {
        libconfig::Config cfg;
        cfg.readFile(file);
        return doParse(cfg);
    } catch (const libconfig::FileIOException& e) {
        LOGE("Unable to read file: {}", e.what());
    } catch (const libconfig::ParseException& e) {
        LOGE("Unable to parse <{}> file on <{}> line: {}", e.getFile(), e.getLine(), e.getError());
    }
    return false;
}

} // namespace jar