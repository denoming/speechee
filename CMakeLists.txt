cmake_minimum_required(VERSION 3.25.1)

include(cmake/ProjectOptions.cmake)

project(speechee VERSION 0.2.1
                 DESCRIPTION "Speechee Service Project")

include(cmake/ProjectConfigs.cmake)
if (PROJECT_IS_TOP_LEVEL)
    include(cmake/ProjectInstall.cmake)
endif()
if (PROJECT_IS_TOP_LEVEL)
    include(cmake/ProjectPackaging.cmake)
endif()

add_subdirectory(config)
add_subdirectory(src)
if (ENABLE_TESTS)
    add_subdirectory(test)
endif ()
add_subdirectory(tools)
