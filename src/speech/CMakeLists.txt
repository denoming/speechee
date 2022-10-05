set(TARGET jarvis-speaker-speech)

add_library(${TARGET} STATIC)
add_library(Jarvis::Speaker::Speech ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_link_libraries(${TARGET}
    PUBLIC
        Boost::headers
        spdlog::spdlog
        google-cloud-cpp::texttospeech
        PkgConfig::SigCpp
    PRIVATE
        PkgConfig::GLib
        PkgConfig::GLibmm
        PkgConfig::Gst
        PkgConfig::GstApp
)

target_sources(${TARGET}
    PRIVATE src/LoggerInitializer.cpp
            src/TextToSpeechClient.cpp
            src/SpeechSynthesizePool.cpp
            src/SpeechSynthesizeTask.cpp
            src/AudioBufferList.cpp
            src/Player.cpp
            src/PlayerLoop.cpp
            src/Speaker.cpp
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)
