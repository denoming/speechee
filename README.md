# Speechee

# Introduction

The speechee service provides synthesizing opportunities of text into speech.

# Build

## By Docker

Get test docker image (only once):
* Build `jarvis-dev-image` yocto image following particular instruction (see `jarvis/doc/build.md`);
* Run `jarvis/scripts/import-dev-image.sh` script and specify dir with build artifacts as argument;
* Run local `scripts/run-test-env.sh` script to create `my/jarvis-dev-image:speechee` test docker image.

Note: The yocto build artifacts located at `jarvis/build-qemu/tmp/deploy/images/qemux86-64` folder and should
contain `jarvis-dev-image-qemux86-64.tar.bz2` file.

Building:
```shell
$ bash scripts/run-test-env.sh
$ cmake -B build-debug-docker -G Ninja
$ cmake --build build-debug-docker --parallel
```

## By SDK

Get SDK installation (only once):
* Build SDK installation file following particular instruction (see `jarvis/doc/build.md`);
* Install SDK using installation file into any dir:
```shell
$ chmod +x jarvis-...-qemux86-64-toolchain-0.1.sh
$ ./jarvis-...-qemux86-64-toolchain-0.1.sh
```

Building:
```shell
$ source <SDK-installation-path>/environment-setup-core2-64-jarvis-linux
$ cmake -B build-debug -G Ninja -DCMAKE_TOOLCHAIN_FILE=toolchain/x86_64-jarsdk-linux.cmake
$ cmake --build build-debug --parallel
```

# Install

```shell
$ cmake --install <build-dir> --prefix <destination-path>/speechee
```

Note: The project should be built previously
