#!/usr/bin/env bash

source dbus-env

TEXT=${1:-"Hello World"}
LANG=${2:-"en-US"}

command -v dbus-send > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "DBus is not installed"
    exit 1
fi

dbus-send --system --dest=${DBUS_SERVICE} --type=method_call \
  ${DBUS_OBJECT} \
  ${DBUS_INTERFACE}.synthesizeText \
  string:"${TEXT}" string:"${LANG}"