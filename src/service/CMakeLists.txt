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

add_library(speechee-service STATIC)
add_library(Speechee::Service ALIAS speechee-service)

target_include_directories(speechee-service
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${SPEECHEE_STAGE_DIR}>
)

target_link_libraries(speechee-service
    PUBLIC Jarvisto::Core
           PkgConfig::SigCpp
           PkgConfig::GLibmm
    PRIVATE gsl::gsl-lite-v1
            PkgConfig::Gst
            PkgConfig::GstApp
            Speechee::Tts
)

target_sources(speechee-service
    PRIVATE src/SpeechSynthesizePool.cpp
            src/SpeechSynthesizeTask.cpp
            src/AudioBufferList.cpp
            src/Player.cpp
            src/PlayerLoop.cpp
            src/PlayerFactory.cpp
            src/Speaker.cpp
            src/SpeakerSubsystem.cpp
            src/GstInitializer.cpp
            src/ServiceLogger.cpp
)

if(ENABLE_DBUS_SUPPORT)
    target_link_libraries(speechee-service
        PRIVATE Speechee::Dbus
    )

    target_sources(speechee-service
        PRIVATE src/SpeakerAdapter.cpp
                src/DbusSpeakerService.cpp
    )
endif()

if(ENABLE_HTTP_SUPPORT)
    target_link_libraries(speechee-service
        PRIVATE Boost::headers
                Boost::json
                Jarvisto::Network
    )

    target_sources(speechee-service
        PRIVATE src/HttpSpeakerService.cpp
    )
endif()

target_compile_features(speechee-service PUBLIC cxx_std_23)
