#include "speechee/DbusSpeakerClient.hpp"

#include "client/DbusSpeakerProxy.hpp"
#include "dbus/Config.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>

#include <gsl/gsl-lite.hpp>

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

namespace jar {

class DbusSpeakerClient::Impl final : public sdbus::ProxyInterfaces<DbusSpeakerProxy> {
public:
    Impl()
        : sdbus::ProxyInterfaces<DbusSpeakerProxy>(dbus::kServiceName, dbus::kObjectPath)
    {
        registerProxy();
    }

    ~Impl() final
    {
        unregisterProxy();
    }

    void
    synthesizeText(const std::string& text, const std::string& lang)
    {
        getProxy()
            .callMethod(dbus::client::SynthesizeText.name)
            .onInterface(dbus::client::SynthesizeText.interface)
            .withArguments(text, lang)
            .dontExpectReply();
    }

    void
    synthesizeSsml(const std::string& text, const std::string& lang)
    {
        getProxy()
            .callMethod(dbus::client::SynthesizeSsml.name)
            .onInterface(dbus::client::SynthesizeSsml.interface)
            .withArguments(text, lang)
            .dontExpectReply();
    }
};

DbusSpeakerClient::DbusSpeakerClient()
    : _impl{std::make_unique<Impl>()}
{
}

DbusSpeakerClient::~DbusSpeakerClient() = default;

void
DbusSpeakerClient::synthesizeText(const std::string& text, const std::string& lang)
{
    gsl_Assert(_impl);
    _impl->synthesizeText(text, lang);
}

void
DbusSpeakerClient::synthesizeSsml(const std::string& ssml, const std::string& lang)
{
    gsl_Assert(_impl);
    _impl->synthesizeSsml(ssml, lang);
}

} // namespace jar
