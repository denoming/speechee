set(TARGET jarvis-speaker-unit-test)

add_executable(${TARGET} "")

target_sources(${TARGET}
    PRIVATE src/SpeechSynthesizePoolTest.cpp
            src/PlayerTest.cpp
            src/SpeakerTest.cpp
)

target_link_libraries(${TARGET}
    PRIVATE Jarvis::Speaker::Test::Common
)

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(${TARGET} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_20)