#include "speaker/ConfigLoader.hpp"

#include <jarvisto/Logger.hpp>
#include <jarvisto/Utils.hpp>

namespace fs = std::filesystem;

namespace jar {

bool
ConfigLoader::load()
{
    bool rv{false};
    if (const auto filePathOpt = getEnvVar("SPEECHEE_CONFIG"); filePathOpt) {
        fs::path filePath{*filePathOpt};
        rv = load(filePath);
    } else {
        LOGE("Set the path to config file using SPEECHEE_CONFIG env variable");
    }
    return rv;
}

bool
ConfigLoader::load(std::string_view str)
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
ConfigLoader::load(std::filesystem::path file)
{
    try {
        libconfig::Config cfg;
        cfg.readFile(file.c_str());
        return doParse(cfg);
    } catch (const libconfig::FileIOException& e) {
        LOGE("Unable to read file: {}", e.what());
    } catch (const libconfig::ParseException& e) {
        LOGE("Unable to parse <{}> file on <{}> line: {}", e.getFile(), e.getLine(), e.getError());
    }
    return false;
}

} // namespace jar