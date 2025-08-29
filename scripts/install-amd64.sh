#!/usr/bin/env bash

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

ONNX_URL=https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-1.21.0.tgz
GSLL_URL=https://github.com/gsl-lite/gsl-lite/archive/refs/tags/v0.42.0.tar.gz
VOXER_URL1=https://github.com/denoming/voxer/releases/download/v0.2.6/libvoxer_0.2.6_amd64.deb
VOXER_URL2=https://github.com/denoming/voxer/releases/download/v0.2.6/libvoxer-dev_0.2.6_amd64.deb
JARVISTO_URL1=https://github.com/denoming/jarvisto/releases/download/v0.3.11/libjarvisto_0.3.11_amd64.deb
JARVISTO_URL2=https://github.com/denoming/jarvisto/releases/download/v0.3.11/libjarvisto-dev_0.3.11_amd64.deb

apt update
apt install -y build-essential autoconf automake autopoint sudo vim git \
               cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
               bison nasm texinfo wget file pkg-config libtool python3 \
               libspdlog-dev libboost1.81-dev libboost-program-options1.81-dev libcxxopts-dev \
               libboost-url1.81-dev libboost-json1.81-dev libhowardhinnant-date-dev \
               libmosquitto-dev libmosquittopp-dev libssl-dev libsigc++-3.0-dev nlohmann-json3-dev \
               libconfig++-dev libglibmm-2.68-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
               gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-tools \
               gstreamer1.0-alsa gstreamer1.0-pulseaudio libespeak-ng-dev

# Install GST Lite
mkdir /tmp/gsl
wget -qO- $GSLL_URL | tar xz -C /tmp/gsl --strip-components=1
cd /tmp/gsl
cmake -B build
cmake --build build --target install
cd -
rm -fr /tmp/gsl

# Install ONNX Runtime Library
mkdir /tmp/onnx
wget -qO- $ONNX_URL | tar xz -C /tmp/onnx --strip-components=1
cd /tmp/onnx
mkdir -p /usr/local/include/onnxruntime
cp -pr include/* /usr/local/include/onnxruntime
cp -pr lib/* /usr/local/lib
cd -
rm -rf /tmp/onnx

# Install libvoxer library
wget -O /tmp/voxer1.deb $VOXER_URL1
apt install -y /tmp/voxer1.deb
rm /tmp/voxer1.deb
wget -O /tmp/voxer2.deb $VOXER_URL2
apt install -y /tmp/voxer2.deb
rm /tmp/voxer2.deb

# Install libjarvisto library
wget -O /tmp/jarvisto1.deb $JARVISTO_URL1
apt install -y /tmp/jarvisto1.deb
rm /tmp/jarvisto1.deb
wget -O /tmp/jarvisto2.deb $JARVISTO_URL2
apt install -y /tmp/jarvisto2.deb
rm /tmp/jarvisto2.deb
