FROM my/jarvis-dev-image

ARG UNAME=dev
ARG UID=1000
ARG GID=1000
ARG PULSE_SERVER=/run/user/$UID/pulse/native

USER root
RUN groupadd -f -g $GID $UNAME
RUN useradd -l -g $GID --uid $UID -ms /bin/bash $UNAME
RUN echo $UNAME:$UNAME | chpasswd
RUN echo '$UNAME ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/$UNAME || true

COPY config/pulse/pulse-client.conf /etc/pulse/client.conf

USER $UNAME

CMD ["/bin/bash"]