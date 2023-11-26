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
synthesizeText(const std::string& text,
               const std::string& lang = "en-US",
               const std::string& host = HttpSpeakerClient::kDefaultHost,
               const std::string& port = HttpSpeakerClient::kDefaultPort)
{
    try {
        HttpSpeakerClient client{host, port};
        client.synthesizeText(text, lang);
    } catch (const std::exception& e) {
        std::cout << "Unable to synthesize text: " << e.what() << std::endl;
    }
}

static void
synthesizeText(const std::filesystem::path& filePath,
               const std::string& text,
               const std::string& lang = "en-US")
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
               const std::string& lang = "en-US",
               const std::string& host = HttpSpeakerClient::kDefaultHost,
               const std::string& port = HttpSpeakerClient::kDefaultPort)
{
    try {
        HttpSpeakerClient client{host, port};
        client.synthesizeSsml(ssml, lang);
    } catch (const std::exception& e) {
        std::cout << "Unable to synthesize ssml: " << e.what() << std::endl;
    }
}

static void
synthesizeSsml(const std::filesystem::path& filePath,
               const std::string& ssml,
               const std::string& lang = "en-US")
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
    std::string host;
    std::string port;
    std::string text;
    std::string ssml;
    std::string lang;
    std::string file;

    po::options_description d{"Speechee CLI"};
    // clang-format off
    d.add_options()
        ("help,h", "Display help")
        ("host,h", po::value<std::string>(&host)->default_value(HttpSpeakerClient::kDefaultHost),
                   "Service host name")
        ("port,p", po::value<std::string>(&port)->default_value(HttpSpeakerClient::kDefaultPort),
                   "Service port")
        ("text,t", po::value<std::string>(&text), "Synthesize given text")
        ("ssml,s", po::value<std::string>(&ssml), "Synthesize given SSML markup")
        ("lang,l", po::value<std::string>(&lang)->default_value("en-US"), "Specifying language")
        ("file,f", po::value<std::string>(&file), "Saving to file")
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
            synthesizeText(text, lang, host, port);
        }
        return EXIT_SUCCESS;
    }

    if (vm.contains("ssml")) {
        if (vm.contains("file")) {
            synthesizeSsml(file, ssml, lang);
        } else {
            synthesizeSsml(ssml, lang, host, port);
        }
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
