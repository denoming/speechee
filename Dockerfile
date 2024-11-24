# syntax=docker/dockerfile:1

ARG PLATFORM=arm64v8
FROM $PLATFORM/debian:bookworm

ARG USER_NAME=bender
ARG USER_UID=1000
ARG USER_GID=1000
ARG VOXER_URL1
ARG VOXER_URL2
ARG JARVISTO_URL1
ARG JARVISTO_URL2
ARG ONNX_URL
ARG PULSE_SERVER="unix:/run/user/$UID/pulse/native"

USER root

RUN apt update \
 && apt install -y build-essential autoconf automake autopoint sudo vim git \
                   cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
                   bison nasm texinfo wget file pkg-config libtool python3 \
                   libspdlog-dev libboost1.81-dev libboost-program-options1.81-dev \
                   libboost-url1.81-dev libboost-json1.81-dev libhowardhinnant-date-dev \
                   libmosquitto-dev libmosquittopp-dev libssl-dev libsigc++-3.0-dev \
                   libconfig++-dev libglibmm-2.68-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
                   gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-tools \
                   gstreamer1.0-alsa gstreamer1.0-pulseaudio

# Install ONNX Runtime Library
RUN mkdir /tmp/onnx \
 && wget -qO- $ONNX_URL | tar xz -C /tmp/onnx --strip-components=1 \
 && cd /tmp/onnx \
 && cp -pr lib/* /lib/aarch64-linux-gnu \
 && cd $HOME \
 && rm -rf /tmp/onnx

# Install libvoxer library
RUN wget -O /tmp/voxer1.deb $VOXER_URL1 \
 && apt install -y /tmp/voxer1.deb \
 && rm /tmp/voxer1.deb \
 && wget -O /tmp/voxer2.deb $VOXER_URL2 \
 && apt install -y /tmp/voxer2.deb \
 && rm /tmp/voxer2.deb

# Install libjarvisto library
RUN wget -O /tmp/jarvisto1.deb $JARVISTO_URL1 \
 && apt install -y /tmp/jarvisto1.deb \
 && rm /tmp/jarvisto1.deb \
 && wget -O /tmp/jarvisto2.deb $JARVISTO_URL2 \
 && apt install -y /tmp/jarvisto2.deb \
 && rm /tmp/jarvisto2.deb

# Create custom user
RUN groupadd -f -g $USER_GID $USER_NAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USER_NAME \
 && echo $USER_NAME:$USER_NAME | chpasswd \
 && echo $USER_NAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USER_NAME || true

# Create dbus folder
RUN mkdir -p /run/dbus

# Copy pulse client config file
COPY config/pulse/pulse-client.conf /etc/pulse/client.conf
# Copy dbus policy config file
COPY config/dbus/org.denoming.speechee.conf /etc/dbus-1/system.d
# Copy entrypoint script
COPY scripts/entrypoint.sh /usr/sbin

USER $USER_NAME

CMD ["/bin/bash","--rcfile","$HOME/.profile"]
