set(TARGET speechee-test-common)

add_library(${TARGET} STATIC)
add_library(Speechee::Test::Common ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE src/TestMain.cpp
            src/MockPlayer.cpp
            src/MockSpeechSynthesizePool.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_link_libraries(${TARGET}
    PUBLIC Speechee::Speaker
           Speechee::Tts
    PUBLIC GTest::gtest GTest::gmock
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)
