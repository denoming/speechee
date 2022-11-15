#include <iostream>

#include "jarvis/SpeakerClient.hpp"

#include <boost/program_options.hpp>

#include <string>

namespace po = boost::program_options;

using namespace jar;

void
synthesizeText(const std::string& text, const std::string& lang = "en-US")
{
    SpeakerClient client;
    client.synthesizeText(text, lang);
}

void
synthesizeSsml(const std::string& ssml, const std::string& lang = "en-US")
{
    SpeakerClient client;
    client.synthesizeSsml(ssml, lang);
}

int
main(int argn, char* argv[])
{
    std::string text;
    std::string ssml;
    std::string lang;

    po::options_description d{"J.A.R.V.I.S Speaker CLI"};
    // clang-format off
    d.add_options()
        ("help,h", "Display help")
        ("text,t", po::value<std::string>(&text), "Synthesize text")
        ("ssml,s", po::value<std::string>(&ssml), "Synthesize SSML")
        ("lang,l", po::value<std::string>(&lang)->default_value("en-US"), "Specify language")
    ;
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argn, argv, d), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << d << std::endl;
        return EXIT_SUCCESS;
    }

    if (vm.count("text")) {
        synthesizeText(text, lang);
        return EXIT_SUCCESS;
    }

    if (vm.count("ssml")) {
        synthesizeSsml(ssml, lang);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}