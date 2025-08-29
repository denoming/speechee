// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef USE_HTTP_CLIENT
#include "speechee/HttpSpeakerClient.hpp"
#else
#include "speechee/DbusSpeakerClient.hpp"
#endif

#include <cxxopts.hpp>

#include <iostream>
#include <string>

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
main(const int argn, char* argv[])
{
    std::string addr;
    std::string port;
    std::string text;
    std::string ssml;
    std::string lang;

    cxxopts::Options options{"Speechee CLI"};

    // clang-format off
    options.add_options()
        ("help,h", "Display help")
#ifdef USE_HTTP_CLIENT
        ("address,a", "Service address", cxxopts::value<std::string>(addr)->default_value(HttpSpeakerClient::kDefaultHost))
        ("port,p", "Service port", cxxopts::value<std::string>(port)->default_value(HttpSpeakerClient::kDefaultPort))
#endif
        ("text,t", "Set text to synthesize", cxxopts::value<std::string>(text))
        ("ssml,s", "Set SSML markup to synthesize", cxxopts::value<std::string>(ssml))
        ("lang,l",  "Set language to use", cxxopts::value<std::string>(lang)->default_value("en-US"))
    ;
    // clang-format on

    try {
        const auto result = options.parse(argn, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return EXIT_SUCCESS;
        }
        if (result.count("text")) {
            synthesizeText(text, lang, addr, port);
            return EXIT_SUCCESS;
        }
        if (result.count("ssml")) {
            synthesizeSsml(ssml, lang, addr, port);
            return EXIT_SUCCESS;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cerr << "Nothing to do, exit." << std::endl;
    return EXIT_FAILURE;
}
