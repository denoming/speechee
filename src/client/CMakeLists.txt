include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

set(TARGET jarvis-speaker-client)

add_library(${TARGET} SHARED)
add_library(Jarvis::Speaker::Client ALIAS ${TARGET})

set_target_properties(${TARGET}
    PROPERTIES
        EXPORT_NAME Speaker
)

set_target_properties(${TARGET}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_sources(${TARGET}
    PRIVATE
        src/SpeakerClient.cpp
        src/SpeakerClientImpl.cpp
)

target_link_libraries(${TARGET}
    PRIVATE
        Jarvis::Speaker::DbusGlue
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

# Generate export header file with export macros
generate_export_header(${TARGET}
    BASE_NAME JARVIS_SPEAKER
    EXPORT_FILE_NAME jarvis/SpeakerExport.hpp
)

# Install target files except namelink file
install(TARGETS ${TARGET}
    EXPORT JarvisSpeakerRuntime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvisSpeakerRuntime
        NAMELINK_SKIP
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install target namelink file
install(TARGETS ${TARGET}
    EXPORT JarvisSpeakerDevelopment
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvisSpeakerDevelopment
        NAMELINK_ONLY
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvisSpeakerDevelopment
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install package config file
install(EXPORT JarvisSpeakerRuntime
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Jarvis
    NAMESPACE Jarvis::
    COMPONENT JarvisSpeakerRuntime
    FILE JarvisSpeakerRuntime.cmake
)

# Install package config file
install(EXPORT JarvisSpeakerDevelopment
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Jarvis
    NAMESPACE Jarvis::
    COMPONENT JarvisSpeakerDevelopment
    FILE JarvisSpeakerDevelopment.cmake
)

# Install library header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    TYPE INCLUDE
    COMPONENT JarvisSpeakerDevelopment
    FILES_MATCHING
        PATTERN *.hpp
)

# Install export header file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvis/SpeakerExport.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvis
    COMPONENT JarvisSpeakerDevelopment
)

# Generate package config and version file
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/cmake/JarvisSpeakerConfig.cmake.in
    JarvisSpeakerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Jarvis
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/JarvisSpeakerConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

# Install package config and version file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvisSpeakerConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Jarvis
    COMPONENT JarvisSpeakerDevelopment
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvisSpeakerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Jarvis
    COMPONENT JarvisSpeakerDevelopment
)