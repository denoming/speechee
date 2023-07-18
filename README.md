# Speechee

## Introduction

The speechee service provides synthesizing text into speech opportunities.
Additionally, integration with HomeAssistant is available.

## Building

### By Docker

Prepare docker image (only once):
* Build `jarvis-dev-image` yocto image following `jarvis/doc/build.md` instruction (see [jarvis](https://github.com/karz0n/jarvis));
* Run `jarvis/scripts/import-dev-image.sh` script and specify dir with build artifacts as argument;

Note: The yocto build artifacts usually located at `jarvis/build-qemu/tmp/deploy/images/qemux86-64` dir
as `jarvis-dev-image-qemux86-64.tar.bz2` file.

Building:
```shell
$ bash scripts/run-test-env.sh
$ cmake -B build-debug-docker -G Ninja
$ cmake --build build-debug-docker --parallel
```

### By SDK

Get SDK installation (only once):
* Build SDK installation file following `jarvis/doc/build.md` instruction (see [jarvis](https://github.com/karz0n/jarvis));
* Install SDK using installation file into any dir:
```shell
$ chmod +x jarvis-...-qemux86-64-toolchain-0.1.sh
$ ./jarvis-...-qemux86-64-toolchain-0.1.sh
```

Note: The SDK installation file usually located at `jarvis/build-qemu/tmp/deploy/sdk` dir
as `jarvis-glibc-*-toolchain-0.1.sh` file.

Building:
```shell
$ source <SDK-installation-path>/environment-setup-core2-64-jarvis-linux
$ cmake -B build-debug -G Ninja
$ cmake --build build-debug --parallel
```

## Installing

```shell
$ cmake --install <build-dir> --prefix <destination-path>/speechee
```

Note: The project should be built previously

## Running

```shell
$ bash scripts/run-test-env.sh
$ build-debug-docker/stage/bin/speechee
...
Start listening on <8080> port
...
```

## Using 

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