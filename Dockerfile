FROM my/jarvis-dev-image

ARG UNAME=dev
ARG UID=1000
ARG GID=1000
ARG PULSE_SERVER="unix:/run/user/$UID/pulse/native"

# Define address to pulse server
ENV PULSE_SERVER=$PULSE_SERVER

# Create default user
USER root
RUN groupadd -f -g $GID $UNAME
RUN useradd -l -g $GID --uid $UID -ms /bin/bash $UNAME
RUN echo $UNAME:$UNAME | chpasswd
RUN echo $UNAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/$UNAME || true

# Create missing folder for dbus
RUN mkdir -p /run/dbus

# Copy pulse client config file
COPY config/pulse/pulse-client.conf /etc/pulse/client.conf
# Copy dbus policy config file
COPY config/dbus/org.denoming.speechee.conf /etc/dbus-1/system.d
# Copy entrypoint script
COPY scripts/entrypoint.sh /usr/sbin

USER $UNAME
ENTRYPOINT ["/bin/bash"]
