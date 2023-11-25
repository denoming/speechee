include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

set(TARGET speechee-client)

add_library(${TARGET} SHARED)
add_library(Speechee::Client ALIAS ${TARGET})

set_target_properties(${TARGET}
    PROPERTIES
        EXPORT_NAME Speechee
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
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(ENABLE_HTTP_SUPPORT)
    target_sources(${TARGET}
        PRIVATE
            src/HttpSpeakerClient.cpp
    )
    target_link_libraries(${TARGET} PRIVATE Boost::headers Boost::json Jarvisto::Jarvisto)
    target_compile_definitions(${TARGET} PRIVATE BOOST_ASIO_NO_DEPRECATED=1)
endif()

if(ENABLE_DBUS_SUPPORT)
    target_sources(${TARGET}
    PRIVATE
        src/DbusSpeakerProxy.cpp
        src/DbusSpeakerClient.cpp
    )
    target_link_libraries(${TARGET} PRIVATE Speechee::Dbus)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_23)

# Generate export header file with export macros
generate_export_header(${TARGET}
    BASE_NAME SPEECHEE
    EXPORT_FILE_NAME org/denoming/speechee/SpeecheeExport.hpp
)

# Install target files except namelink file
install(TARGETS ${TARGET}
    EXPORT SpeecheeRuntime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT SpeecheeRuntime
        NAMELINK_SKIP
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install target namelink file
install(TARGETS ${TARGET}
    EXPORT SpeecheeDevelopment
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT SpeecheeDevelopment
        NAMELINK_ONLY
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT SpeecheeDevelopment
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install package config file
install(EXPORT SpeecheeRuntime
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/org/denoming/speechee
    NAMESPACE Speechee::
    COMPONENT SpeecheeRuntime
    FILE SpeecheeRuntime.cmake
)

# Install package config file
install(EXPORT SpeecheeDevelopment
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/org/denoming/speechee
    NAMESPACE Speechee::
    COMPONENT SpeecheeDevelopment
    FILE SpeecheeDevelopment.cmake
)

# Install library header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    TYPE INCLUDE
    COMPONENT SpeecheeDevelopment
    FILES_MATCHING
        PATTERN *.hpp
)

# Install export header file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org/denoming/speechee/SpeecheeExport.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/org/denoming/speechee
    COMPONENT SpeecheeDevelopment
)

# Generate package config and version file
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/cmake/SpeecheeConfig.cmake.in SpeecheeConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/org/denoming/speechee
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SpeecheeConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

# Install package config and version file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SpeecheeConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/org/denoming/Speechee
    COMPONENT SpeecheeDevelopment
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SpeecheeConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/org/denoming/Speechee
    COMPONENT SpeecheeDevelopment
)
