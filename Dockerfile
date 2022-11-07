FROM my/jarvis-dev-image

ARG UID=1000
ARG GID=1000
ARG PULSE_SERVER="unix:/run/user/$UID/pulse/native"

# Define address to pulse server
ENV PULSE_SERVER=$PULSE_SERVER

# Create default user
USER root
RUN groupadd -f -g $GID dev
RUN useradd -l -g $GID --uid $UID -ms /bin/bash dev
RUN echo dev:dev | chpasswd
RUN echo 'dev ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/dev || true
# Create missing folder for dbus
RUN mkdir -p /run/dbus

# Copy pulse client config file
COPY config/pulse/pulse-client.conf /etc/pulse/client.conf
# Copy dbus policy config file
COPY config/dbus/org.denoming.jarvis.speaker.conf /etc/dbus-1/system.d
# Copy run script
COPY scripts/run.sh /usr/sbin

USER dev
ENTRYPOINT ["/bin/bash"]