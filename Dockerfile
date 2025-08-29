# syntax=docker/dockerfile:1

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

ARG BASE_CONTAINER=python:3.12-bookworm
FROM $BASE_CONTAINER
ARG TARGETARCH

ARG USERNAME=bender
ARG USER_UID=1000
ARG USER_GID=1000
ARG PULSE_SERVER="unix:/run/user/$USER_UID/pulse/native"

# Install dependencies
ADD scripts/install-$TARGETARCH.sh /tmp
RUN chmod +x /tmp/install-$TARGETARCH.sh
RUN /tmp/install-$TARGETARCH.sh

# Create custom user
RUN groupadd -f -g $USER_GID $USERNAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USERNAME \
 && echo $USERNAME:$USERNAME | chpasswd \
 && echo $USERNAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USERNAME || true

# Create dbus folder
RUN mkdir -p /run/dbus

# Copy pulse client config file
COPY config/pulse/pulse-client.conf /etc/pulse/client.conf
# Copy dbus policy config file
COPY config/dbus/org.denoming.speechee.conf /etc/dbus-1/system.d
# Copy entrypoint script
COPY scripts/entrypoint.sh /usr/sbin
