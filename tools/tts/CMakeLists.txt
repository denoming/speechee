set(TARGET speechee-tts)

add_library(${TARGET} STATIC)
add_library(Speechee::Tts ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${TARGET}
    PRIVATE Boost::headers
            google-cloud-cpp::texttospeech
)

target_sources(${TARGET}
    PRIVATE src/TextToSpeechClient.cpp
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)
