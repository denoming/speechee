cmake_minimum_required(VERSION 3.22)

include(cmake/ProjectOptions.cmake)

project(speechee
    VERSION 0.2.1
    DESCRIPTION "Speechee Project"
)

include(cmake/ProjectConfigs.cmake)
include(cmake/ProjectInstall.cmake)

add_subdirectory(config)
add_subdirectory(src)
if (ENABLE_TESTS)
    add_subdirectory(test)
endif ()
add_subdirectory(tools)
