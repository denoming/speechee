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
