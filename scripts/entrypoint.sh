#!/usr/bin/env bash

if ! pgrep -x "dbus-daemon" > /dev/null
then
    # Generate machine-id
    sudo sh -c 'dbus-uuidgen > /var/lib/dbus/machine-id'
    # Run dbus daemon
    export DBUS_SYSTEM_BUS_ADDRESS=$(sudo dbus-daemon --fork --config-file=/usr/share/dbus-1/system.conf --print-address)
else
    echo "dbus-daemon already running"
fi

exec bash