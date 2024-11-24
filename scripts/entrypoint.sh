#!/usr/bin/env bash

if ! pgrep -x "dbus-daemon" > /dev/null
then
    # Run dbus daemon
    BUS=$(dbus-daemon --fork --config-file=/usr/share/dbus-1/session.conf --print-address)
    # Export bus address
    export DBUS_SESSION_BUS_ADDRESS=$BUS
else
    echo "dbus-daemon already running"
fi

exec bash --rcfile "$HOME/.profile"
