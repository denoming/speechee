FROM python:3.12-bookworm

ARG UNAME=dev
ARG UID=1000
ARG GID=1000
ARG PULSE_SERVER="unix:/run/user/$UID/pulse/native"

ENV VCPKG_ROOT="/home/$UNAME/.vcpkg"
ENV PULSE_SERVER=$PULSE_SERVER

USER root

RUN apt update && \
    apt install -y build-essential autoconf automake autopoint sudo vim git cmake ninja-build gdb curl tar zip \
                   unzip sudo dbus flex bison nasm texinfo \
                   libpulse-dev

# Create default user
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

RUN pip install Jinja2

RUN git clone https://github.com/Microsoft/vcpkg.git $HOME/.vcpkg && \
    bash $HOME/.vcpkg/bootstrap-vcpkg.sh && \
    mkdir -p $HOME/.local/bin && \
    ln -s $HOME/.vcpkg/vcpkg $HOME/.local/bin/vcpkg

CMD bash --rcfile "$HOME/.profile"
