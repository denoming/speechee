include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/SpeecheeConfig.cmake.in
        SpeecheeConfig.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/speechee
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/SpeecheeConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SpeecheeConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speechee
    COMPONENT SpeecheeDevelopment
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SpeecheeConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speechee
    COMPONENT SpeecheeDevelopment
)