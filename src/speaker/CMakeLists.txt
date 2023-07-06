set(TARGET speechee-speaker)

add_library(${TARGET} STATIC)
add_library(Speechee::Speaker ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
            $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/options>
)

target_link_libraries(${TARGET}
    PUBLIC Jarvisto::Jarvisto
           PkgConfig::SigCpp
    PRIVATE Boost::headers
            Boost::json
            PkgConfig::GLibmm
            PkgConfig::Gst
            PkgConfig::GstApp
            Speechee::Tts
            Speechee::Dbus
)

target_sources(${TARGET}
    PRIVATE src/Types.cpp
            src/SpeechSynthesizePool.cpp
            src/SpeechSynthesizeTask.cpp
            src/AudioBufferList.cpp
            src/Player.cpp
            src/PlayerLoop.cpp
            src/Speaker.cpp
            src/SpeakerAdapter.cpp
            src/SpeakerSubsystem.cpp
            src/DbusSpeakerService.cpp
            src/HttpSpeakerService.cpp
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)
