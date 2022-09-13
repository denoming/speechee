set(TARGET jarvis-speaker-unit-test)

add_executable(${TARGET} "")

target_sources(${TARGET}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/test.cpp
)

target_link_libraries(
    ${TARGET}
    PRIVATE GTest::gtest_main GTest::gmock_main
)

if (NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(${TARGET} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

