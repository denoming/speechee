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

include(GNUInstallDirs)

if(NOT SYSTEMD_UNITS_INSTALL_DIR)
    set(SYSTEMD_UNITS_INSTALL_DIR "${CMAKE_INSTALL_SYSCONFDIR}/systemd/system" CACHE FILEPATH "Systemd units path" FORCE)
endif()
message(VERBOSE "Systemd units path: ${SYSTEMD_UNITS_INSTALL_DIR}")

configure_file(speechee.service.in speechee.service @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/speechee.service
    DESTINATION ${SYSTEMD_UNITS_INSTALL_DIR}
    COMPONENT Speechee_Runtime
)
