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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

if (EXISTS "$ENV{HOME}/.local")
    list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/.local)
endif()

if (DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Toolchain file ${CMAKE_TOOLCHAIN_FILE}")
endif()

include(BuildType)
include(BuildLocation)
include(BuildOptions)

if (ENABLE_TESTS)
    enable_testing()
    include(AddGoogleTest)
    message(VERBOSE "Building with tests")
else()
    message(VERBOSE "Building without tests")
endif()

if(NOT SYSTEMD_UNAME)
    set(SYSTEMD_UNAME "speechee" CACHE STRING "User to run service as" FORCE)
endif()
message(VERBOSE "Systemd user: ${SYSTEMD_UNAME}")
if(NOT SYSTEMD_GNAME)
    set(SYSTEMD_GNAME "speechee" CACHE STRING "Group to run service as" FORCE)
endif()
message(VERBOSE "Systemd group: ${SYSTEMD_GNAME}")

include(AddGstLite)
if(ENABLE_HTTP_SUPPORT)
    include(AddBoost)
endif()
if(ENABLE_CLI)
    include(AddCxxOpts)
endif()
include(AddSigCpp)
include(AddGLibmm)
include(AddGst)
include(AddJarvisto)
if(ENABLE_ONLINE_TTS)
    include(AddGoogleTts)
endif()
if(ENABLE_DBUS_SUPPORT)
    include(AddSdbusCpp)
endif()
if(ENABLE_OFFLINE_TTS)
    include(AddVoxer)
endif()
