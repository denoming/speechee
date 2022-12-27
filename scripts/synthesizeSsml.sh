#!/usr/bin/env bash

PROJECT_DIR=$(dirname "$(dirname "$(realpath -s $0)")")

source $PROJECT_DIR/scripts/dbus-env

TEXT=${1:-"<speak>Hello World with <say-as interpret-as='characters'>SSML</say-as></speak>"}
LANG=${2:-"en-US"}

if ! pgrep -x "jarvis-speaker" > /dev/null
then
    echo -e "Error:"
    echo -e "J.A.R.V.I.S Speaker process is not active"
    exit 1
fi

command -v dbus-send > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "DBus is not installed"
    exit 1
fi

dbus-send --system --dest=${DBUS_SERVICE} --type=method_call \
  ${DBUS_OBJECT} \
  ${DBUS_INTERFACE}.synthesizeSsml \
  string:"${TEXT}" string:"${LANG}"