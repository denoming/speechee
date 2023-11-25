set(TARGET speechee-unit-test)

add_executable(${TARGET} "")

target_sources(${TARGET}
    PRIVATE src/SpeechSynthesizePoolTest.cpp
            src/PlayerTest.cpp
            src/SpeakerTest.cpp
            src/AvailabilityObserverTest.cpp
            src/GeneralCofigTest.cpp
)

target_link_libraries(${TARGET}
    PRIVATE Speechee::Test::Common
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT SpeecheeRuntime
)

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(${TARGET}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PROPERTIES LABELS "Unit"
    )
endif()
