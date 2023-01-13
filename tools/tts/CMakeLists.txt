set(TARGET jarvis-speaker-tts)

find_package(google_cloud_cpp_texttospeech REQUIRED)

add_library(${TARGET} STATIC)
add_library(Jarvis::Speaker::Tts ALIAS ${TARGET})

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

target_compile_features(${TARGET} PUBLIC cxx_std_20)
