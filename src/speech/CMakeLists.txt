set(TARGET jarvis-speaker-speech)

add_library(${TARGET} STATIC)
add_library(Jarvis::Speaker::Speech ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_link_libraries(${TARGET}
    PRIVATE
        Boost::headers
        spdlog::spdlog
        google-cloud-cpp::texttospeech
)

target_sources(${TARGET}
    PRIVATE src/LoggerInitializer.cpp
            src/SpeechSynthesizer.cpp
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)
