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

add_library(speechee-tts STATIC)
add_library(Speechee::Tts ALIAS speechee-tts)

target_include_directories(speechee-tts
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(speechee-tts
    PRIVATE gsl::gsl-lite-v1
)

if(ENABLE_ONLINE_TTS)
    target_link_libraries(speechee-tts PRIVATE google-cloud-cpp::texttospeech)
    target_sources(speechee-tts PRIVATE src/OnlineTextToSpeechClient.cpp)
endif()
if(ENABLE_OFFLINE_TTS)
    target_link_libraries(speechee-tts PRIVATE Voxer::Voxer)
    target_sources(speechee-tts PRIVATE src/OfflineTextToSpeechEngine.cpp)
endif()

target_compile_features(speechee-tts PUBLIC cxx_std_23)
