# Copyright 2025 Denys Asauliak
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

DOMAIN = "speechee"

# Config
CONF_URL = "url"

# Arguments
ARG_TEXT = "text"
ARG_LANG = "lang"

import requests
from homeassistant.config_entries import ConfigEntry
from homeassistant.core import HomeAssistant, ServiceCall, ServiceResponse


def setup(hass: HomeAssistant, config: ConfigEntry):
    def synthesize_text(call: ServiceCall) -> ServiceResponse:
        url = str(config[DOMAIN][CONF_URL]) + "/synthesize-text"
        lang = str(call.data.get(ARG_LANG)[0:5])
        text = str(call.data.get(ARG_TEXT)[0:2000])
        json = {"text": text, "lang": lang}
        r = requests.post(url, json=json, timeout=15)
        return {
            "code": int(r.status_code),
        }

    def synthesize_ssml(call: ServiceCall) -> ServiceResponse:
        url = str(config[DOMAIN][CONF_URL]) + "/synthesize-ssml"
        lang = str(call.data.get(ARG_LANG)[0:5])
        text = str(call.data.get(ARG_TEXT)[0:2000])
        json = {"text": text, "lang": lang}
        r = requests.post(url, json=json, timeout=15)
        return {
            "code": int(r.status_code),
        }

    hass.services.register(DOMAIN, "synthesize_text", synthesize_text)
    hass.services.register(DOMAIN, "synthesize_ssml", synthesize_ssml)
    return True
