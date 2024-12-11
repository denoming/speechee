include(GNUInstallDirs)

if(NOT SYSTEMD_UNITS_INSTALL_DIR)
    set(SYSTEMD_UNITS_INSTALL_DIR "${CMAKE_INSTALL_SYSCONFDIR}/systemd/system" CACHE FILEPATH "Systemd units path" FORCE)
endif()
message(VERBOSE "Systemd units path: ${SYSTEMD_UNITS_INSTALL_DIR}")

configure_file(speechee.service.in speechee.service @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/speechee.service
    DESTINATION ${SYSTEMD_UNITS_INSTALL_DIR}
    COMPONENT Speechee_Runtime
)
