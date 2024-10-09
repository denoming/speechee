
if (NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Set default install prefix path
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/opt/denoming.com/${PROJECT_NAME}"
                CACHE PATH "Default install prefix" FORCE)
    endif()
endif()

# Copy asset folder to the stage directory
file(COPY ${PROJECT_SOURCE_DIR}/asset DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
