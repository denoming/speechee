add_executable(speechee-unit-test "")

target_sources(speechee-unit-test
    PRIVATE src/SpeechSynthesizePoolTest.cpp
            src/PlayerTest.cpp
            src/SpeakerTest.cpp
)

target_link_libraries(speechee-unit-test
    PRIVATE Speechee::Test::Common
)

target_compile_features(speechee-unit-test PUBLIC cxx_std_23)

install(
    TARGETS speechee-unit-test
    COMPONENT Speechee_Runtime
)

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(speechee-unit-test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PROPERTIES LABELS "Unit"
    )
endif()
