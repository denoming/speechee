#!/usr/bin/env bash

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
