#ifdef USE_HTTP_CLIENT
#include "speechee/HttpSpeakerClient.hpp"
#else
#include "speechee/DbusSpeakerClient.hpp"
#endif

#include <boost/program_options.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
namespace po = boost::program_options;

using namespace jar;

static void
synthesizeText(const std::string& text,
               const std::string& lang,
               const std::string& addr,
               const std::string& port)
{
    try {
#ifdef USE_HTTP_CLIENT
        HttpSpeakerClient client{addr, port};
#else
        DbusSpeakerClient client;
#endif
        client.synthesizeText(text, lang);
    } catch (const std::exception& e) {
        std::cout << "Unable to synthesize text: " << e.what() << std::endl;
    }
}

static void
synthesizeSsml(const std::string& ssml,
               const std::string& lang,
               const std::string& addr,
               const std::string& port)
{
    try {
#ifdef USE_HTTP_CLIENT
        HttpSpeakerClient client{addr, port};
#else
        DbusSpeakerClient client;
#endif
        client.synthesizeSsml(ssml, lang);
    } catch (const std::exception& e) {
        std::cout << "Unable to synthesize SSML: " << e.what() << std::endl;
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

    po::options_description d{"Speechee CLI"};
    // clang-format off
    d.add_options()
        ("help,h", "Display help")
#ifdef USE_HTTP_CLIENT
        ("address,a", po::value<std::string>(&addr)->default_value(HttpSpeakerClient::kDefaultHost),
                   "Service address")
        ("port,p", po::value<std::string>(&port)->default_value(HttpSpeakerClient::kDefaultPort),
                   "Service port")
#endif
        ("text,t", po::value<std::string>(&text), "Set text to synthesize")
        ("ssml,s", po::value<std::string>(&ssml), "Set SSML markup to synthesize")
        ("lang,l", po::value<std::string>(&lang)->default_value("en-US"), "Set language to use")
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
        synthesizeText(text, lang, addr, port);
        return EXIT_SUCCESS;
    }

    if (vm.contains("ssml")) {
        synthesizeSsml(ssml, lang, addr, port);
        return EXIT_SUCCESS;
    }

    std::cerr << "Nothing to do, exit." << std::endl;
    return EXIT_FAILURE;
}
