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

add_library(speechee-test-common STATIC)
add_library(Speechee::Test::Common ALIAS speechee-test-common)

target_sources(speechee-test-common
    PRIVATE src/MockPlayer.cpp
            src/MockPlayerFactory.cpp
            src/MockSpeechSynthesizePool.cpp
)

target_include_directories(speechee-test-common
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_link_libraries(speechee-test-common
    PUBLIC Speechee::Service
           Speechee::Tts
           Speechee::Test
)

target_compile_features(speechee-test-common PUBLIC cxx_std_23)
