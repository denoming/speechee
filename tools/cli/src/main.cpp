#include "tts/TextToSpeechClient.hpp"

#include <org/denoming/speechee/HttpSpeakerClient.hpp>

#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
namespace po = boost::program_options;

using namespace jar;

static void
saveToFile(const fs::path& filePath, std::string_view audioData)
{
    std::fstream file{filePath, std::ios::out | std::ios::binary};
    if (not file) {
        std::cerr << "Unable to open '" << filePath << "' file" << std::endl;
        return;
    }
    file.write(audioData.data(), std::streamsize(audioData.size()));
    file.flush();
    file.close();
}

static void
synthesizeText(const std::string& text,
               const std::string& lang,
               const std::string& addr,
               const std::string& port)
{
    try {
        HttpSpeakerClient client{addr, port};
        client.synthesizeText(text, lang);
    } catch (const std::exception& e) {
        std::cout << "Unable to synthesize text: " << e.what() << std::endl;
    }
}

static void
synthesizeText(const fs::path& filePath, const std::string& text, const std::string& lang)
{
    jar::TextToSpeechClient client;
    std::error_code ec;
    if (std::string audio = client.synthesizeText(text, lang, ec); ec) {
        std::cerr << "Unable to synthesize text: " << ec.message() << std::endl;
    } else {
        std::cout << "Saving to '" << filePath.string() << "' file" << std::endl;
        saveToFile(filePath, audio);
    }
}

static void
synthesizeSsml(const std::string& ssml,
               const std::string& lang,
               const std::string& addr,
               const std::string& port)
{
    try {
        HttpSpeakerClient client{addr, port};
        client.synthesizeSsml(ssml, lang);
    } catch (const std::exception& e) {
        std::cout << "Unable to synthesize SSML: " << e.what() << std::endl;
    }
}

static void
synthesizeSsml(const fs::path& filePath, const std::string& ssml, const std::string& lang)
{
    jar::TextToSpeechClient client;
    std::error_code ec;
    if (std::string audio = client.synthesizeSsml(ssml, lang, ec); ec) {
        std::cerr << "Unable to synthesize ssml: " << ec.message() << std::endl;
    } else {
        std::cout << "Saving to '" << filePath.string() << "' file" << std::endl;
        saveToFile(filePath, audio);
    }
}

int
main(int argn, char* argv[])
{
    std::string addr;
    std::string port;
    std::string text;
    std::string ssml;
    std::string lang;
    std::string file;

    po::options_description d{"Speechee CLI"};
    // clang-format off
    d.add_options()
        ("help,h", "Display help")
        ("address,a", po::value<std::string>(&addr)->default_value(HttpSpeakerClient::kDefaultHost),
                   "Service address")
        ("port,p", po::value<std::string>(&port)->default_value(HttpSpeakerClient::kDefaultPort),
                   "Service port")
        ("text,t", po::value<std::string>(&text), "Set text to synthesize")
        ("ssml,s", po::value<std::string>(&ssml), "Set SSML markup to synthesize")
        ("lang,l", po::value<std::string>(&lang)->default_value("en-US"), "Set language to use")
        ("file,f", po::value<std::string>(&file), "Set file to save audio")
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
            synthesizeText(text, lang, addr, port);
        }
        return EXIT_SUCCESS;
    }

    if (vm.contains("ssml")) {
        if (vm.contains("file")) {
            synthesizeSsml(file, ssml, lang);
        } else {
            synthesizeSsml(ssml, lang, addr, port);
        }
        return EXIT_SUCCESS;
    }

    std::cerr << "Nothing to do, exit." << std::endl;
    return EXIT_FAILURE;
}
