# Setup

Configuration includes two files:
* `speechee.cfg` main config file
* `speechee-cloud-access.json` cloud credentials config file

## Main config file

The `speechee.cfg` config file contains main configration needed to start service properly and contains
following information:
* HTTP API service configration
* MQTT access configration
* Main synthesis service configration

Example of such configuration file is following:
```
synthesis =
{
    threads = 2;
};

mqtt =
{
    user = "denys";
    password = "123456";
    server = "192.168.1.43";
};

services =
{
    http =
    {
        port = 2350;
        threads = 2;
    };
};
```

The location of this file is getting from `SPEECHEE_CONFIG` environment variable.

## Cloud config file

Cloud credentials config file represents a JSON file. This JSON file contains Google TTS cloud credentials.
The path to this file should be specified by `GOOGLE_APPLICATION_CREDENTIALS` environment variable.