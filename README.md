# Speechee

## Introduction

The speechee service provides synthesizing text into speech opportunities.

The main features of this service are following:
* synthesizing plain text into speech and play on default audio device
* synthesizing SSML Google TTS into speech and play on default audio device

The integration with HomeAssistant as custom component is provided additionally. Using this integration you will
be able to use synthesizing feature in your automation.

## Building

### By vcpkg (locally)

Debug configuration:
```shell
$ cmake --preset debug
$ cmake --build --preset build-debug
```
Release configuration:
```shell
$ cmake --preset release
$ cmake --build --preset build-release
```

### By vcpkg (in docker)

Debug configuration:
```shell
$ bash scripts/run-test-env.sh
$ cmake --preset debug-docker
$ cmake --build --preset build-debug-docker
```
Release configuration:
```shell
$ bash scripts/run-test-env.sh
$ cmake --preset release-docker
$ cmake --build --preset build-release-docker
```

## Installing

```shell
$ cmake --install <build-dir> --prefix <destination-path>/speechee
```

Note: The project should be built previously.

## Running

### Locally

To run speechee service locally the following environment variable should be set:
* `SPEECHEE_CONFIG` - main config file
* `GOOGLE_APPLICATION_CREDENTIALS` - cloud credentials config file
* `GST_PLUGIN_SCANNER` - path to `gst-plugin-scanner` tool in build directory
* `GST_PLUGIN_PATH` - path to dir with gstreamer plugins in build directory

Example:
```shell
$ export SPEECHEE_CONFIG=$HOME/.local/share/speechee/speechee.cfg
$ export GOOGLE_APPLICATION_CREDENTIALS=$HOME/.local/share/speechee/speechee-cloud-access.json
$ export GST_PLUGIN_SCANNER=$PWD/build-debug/vcpkg_installed/x64-linux-dynamic/tools/gstreamer/gst-plugin-scanner
$ export GST_PLUGIN_PATH=$PWD/build-debug/vcpkg_installed/x64-linux-dynamic/debug/plugins/gstreamer

After above environment variables are set the service can be run:
```shell
$ build-debug/stage/bin/speechee
...
Start listening on <2350> port
```

Note: The service should be built previously.

### Inside docker

```shell
$ bash scripts/run-test-env.sh
$ build-debug-docker/stage/bin/speechee
...
Start listening on <2350> port
```

Note: The service should be built previously.

## Using

### By CLI

Synthesize text request example:
```shell
$ build-debug/stage/bin/speechee-cli --text "Hello World"
```

Synthesize SSML text request example:
```shell
$ build-debug/stage/bin/speechee-cli --ssml "<speak>Hello <break time=\"1s\"/> World</speak>"
```

### By CURL

Synthesize text request example:
```bash
curl -X POST http://<ip-address>:8080/synthesize-text \
  -H "Content-Type: application/json" \
  -d '{"text": "Hello World", "lang": "en-US"}'
```

Synthesize SSML text request example:
```bash
curl -X POST http://<ip-address>:8080/synthesize-ssml \
  -H "Content-Type: application/json" \
  -d '{"value": "<speak>Hello <break time=\"1s\"/> World</speak>", "lang": "en-US"}'
```

Bad request example 1:
```bash
curl -X POST http://<ip-address>:8080/synthesize-ssml \
  -H "Content-Type: application/json" \
  -d '{"body": "Hello World"}'
```

Bad request example 2:
```bash
curl -X GET http://<ip-address>:8080/synthesize-ssml
```

Note: Change `<ip-address>` to particular IP address of server where `speechee` service is running.

## License

See the [LICENSE](LICENSE.md) file for license rights and limitations (MIT).