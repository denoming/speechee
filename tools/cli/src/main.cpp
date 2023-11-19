#include "tts/TextToSpeechClient.hpp"

#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
namespace po = boost::program_options;

static void
saveToFile(const fs::path& filePath, std::string_view audioData)
{
    std::fstream stream{filePath, std::ios::out | std::ios::binary};
    if (not stream) {
        std::cerr << "Unable to open '" << filePath << "' file" << std::endl;
        return;
    }
    stream.write(audioData.data(), std::streamsize(audioData.size()));
    stream.flush();
    stream.close();
}

static void
synthesizeText(const std::string& /*text*/, const std::string& /*lang*/ = "en-US")
{
    std::cout << "API Not supported yet" << std::endl;
}

static void
synthesizeText(const std::filesystem::path& filePath,
               const std::string& text,
               const std::string& lang = "en-US")
{
    std::error_code error;
    jar::TextToSpeechClient client;
    std::string audio = client.synthesizeText(text, lang, error);
    if (error) {
        std::cerr << "Unable to synthesize text: " << error.message() << std::endl;
    } else {
        std::cout << "Saving to '" << filePath.string() << "' file" << std::endl;
        saveToFile(filePath, audio);
    }
}

static void
synthesizeSsml(const std::string& /*ssml*/, const std::string& /*lang*/ = "en-US")
{
    std::cout << "API Not supported yet" << std::endl;
}

static void
synthesizeSsml(const std::filesystem::path& filePath,
               const std::string& ssml,
               const std::string& lang = "en-US")
{
    std::error_code error;
    jar::TextToSpeechClient client;
    std::string audio = client.synthesizeSsml(ssml, lang, error);
    if (error) {
        std::cerr << "Unable to synthesize ssml: " << error.message() << std::endl;
    } else {
        std::cout << "Saving to '" << filePath.string() << "' file" << std::endl;
        saveToFile(filePath, audio);
    }
}

int
main(int argn, char* argv[])
{
    std::string text;
    std::string ssml;
    std::string lang;
    std::string file;

    po::options_description d{"Speechee CLI"};
    // clang-format off
    d.add_options()
        ("help,h", "Display help")
        ("text,t", po::value<std::string>(&text), "Synthesize text")
        ("ssml,s", po::value<std::string>(&ssml), "Synthesize SSML")
        ("lang,l", po::value<std::string>(&lang)->default_value("en-US"), "Specify language")
        ("file,f", po::value<std::string>(&file), "Save to file")
    ;
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argn, argv, d), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << d << std::endl;
        return EXIT_SUCCESS;
    }

    if (vm.contains("text")) {
        if (vm.contains("file")) {
            synthesizeText(file, text, lang);
        } else {
            synthesizeText(text, lang);
        }
        return EXIT_SUCCESS;
    }

    if (vm.contains("ssml")) {
        if (vm.contains("file")) {
            synthesizeSsml(file, ssml, lang);
        } else {
            synthesizeSsml(ssml, lang);
        }
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
