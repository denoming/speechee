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

add_executable(speechee-cli "")
add_executable(Speechee::Speaker::Cli ALIAS speechee-cli)

target_sources(speechee-cli
    PRIVATE
        src/main.cpp
)

target_link_libraries(speechee-cli
    PRIVATE cxxopts::cxxopts
            Speechee::Tts
            Speechee::Client
)

target_include_directories(speechee-cli
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_compile_features(speechee-cli PRIVATE cxx_std_23)

install(
    TARGETS speechee-cli
    COMPONENT Speechee_Runtime
)
