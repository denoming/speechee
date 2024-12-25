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
