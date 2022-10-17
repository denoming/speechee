set(TARGET jarvis-speaker-test-common)

add_library(${TARGET} STATIC)
add_library(Jarvis::Speaker::Test::Common ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE src/MockTextToSpeechClient.cpp
            src/Waiter.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_link_libraries(${TARGET}
    PUBLIC Jarvis::Speaker::Speech
    PUBLIC GTest::gtest GTest::gmock
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)
