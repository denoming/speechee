# Speechee

## Introduction

The speechee service provides synthesizing text into speech opportunities.

The main features of this service are following:
* synthesizing plain text into speech and play on default audio device
* synthesizing SSML Google TTS into speech and play on default audio device

The integration with HomeAssistant as custom component is provided additionally. Using this integration you will
be able to use synthesizing feature in your automation.

## Building

Output artifacts default location is `<build-dir>/stage`.

Packing artifacts includes the following files:

* `libvoxer_<version>_.deb`
* `libvoxer-dev_<version>_.deb`
* (optional) `libvoxer-data_<version>_.deb`

### Locally

Build and packing:

```shell
# Debug
$ cmake --workflow --fresh --preset debug
# Release
$ cmake --workflow --fresh --preset release
```

### By Docker

Build and packing:

```shell
# Debug
$ cmake --workflow --fresh --preset debug-docker
# Release
$ cmake --workflow --fresh --preset release-docker
```

## Installing

```shell
$ cmake --install <build-dir> --prefix <destination-path>/speechee
```

## Testing

```shell
$ bash scripts/run-build-env.sh
$ cmake --preset debug-docker
$ cmake --build --preset debug-docker
$ ctest --preset unit-tests
```

Note: The project should be built previously.

## Usage

### Run offline TTS service

Run service:
```shell
$ export JAR_CONFIG_DIR="$HOME/.local/share/speechee/config"
$ <build-dir>/stage/bin/speechee
```

### Run online TTS service

Set environment variable:
* `JAR_CONFIG_DIR` - dir with config files
* `GOOGLE_APPLICATION_CREDENTIALS` - cloud credentials config file

Example:
```shell
$ export JAR_CONFIG_DIR="$HOME/.local/share/speechee/config"
$ export GOOGLE_APPLICATION_CREDENTIALS=$HOME/.local/share/speechee/speechee-cloud-access.json
$ build-debug/stage/bin/speechee
```

### Use voxer CLI

Synthesize text request example:
```shell
$ build-debug/stage/bin/speechee-cli --text "Hello World"
```

Synthesize SSML text request example:
```shell
$ build-debug/stage/bin/speechee-cli --ssml "<speak>Hello <break time=\"1s\"/> World</speak>"
```

### Use CURL

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