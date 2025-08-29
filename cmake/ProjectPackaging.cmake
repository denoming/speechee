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

include(CPackComponent)

# Set default packaging install prefix path
set(CPACK_PACKAGING_INSTALL_PREFIX "/")

set(CPACK_PACKAGE_NAME "speechee")
set(CPACK_PACKAGE_CONTACT "maintainer@denoming.com")
set(CPACK_PACKAGE_VENDOR "DENOMING")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Speechee Project")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_COMPONENTS_ALL
        Speechee_Runtime
)
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES
        /\\.git/
        /\\.idea/
        /build.*/
)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

if(EXISTS "${PROJECT_SOURCE_DIR}/DESCRIPTION.md")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/DESCRIPTION.md")
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/WELCOME.md")
    set(CPACK_RESOURCE_FILE_WELCOME "${PROJECT_SOURCE_DIR}/WELCOME.md")
endif()
if (EXISTS "${PROJECT_SOURCE_DIR}/LICENSE.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md")
endif()
if (EXISTS "${PROJECT_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR "TGZ;DEB")
    include(cmake/generators/deb.cmake)
    include(cmake/generators/tgz.cmake)
else()
    set(CPACK_GENERATOR "TGZ")
    include(cmake/generators/tgz.cmake)
endif()

cpack_add_component(Speechee_Runtime
        DISPLAY_NAME Runtime
        DESCRIPTION "Speechee runtime"
        REQUIRED
)

include(CPack)
