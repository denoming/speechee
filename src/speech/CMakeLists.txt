set(TARGET jarvis-speaker-speech)

add_library(${TARGET} STATIC)
add_library(Jarvis::Speaker::Speech ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_link_libraries(${TARGET}
    PUBLIC Jarvis::Common
    PUBLIC
        PkgConfig::SigCpp
    PRIVATE
        Boost::headers
        PkgConfig::GLib
        PkgConfig::GLibmm
        PkgConfig::Gst
        PkgConfig::GstApp
        google-cloud-cpp::texttospeech
        Jarvis::Speaker::Dbus
)

target_sources(${TARGET}
    PRIVATE src/Types.cpp
            src/TextToSpeechClient.cpp
            src/SpeechSynthesizePool.cpp
            src/SpeechSynthesizeTask.cpp
            src/AudioBufferList.cpp
            src/Player.cpp
            src/PlayerLoop.cpp
            src/Speaker.cpp
            src/SpeakerService.cpp
            src/SpeechSubsystem.cpp
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)
