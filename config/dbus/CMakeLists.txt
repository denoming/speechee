include(GNUInstallDirs)

install(FILES ${CMAKE_CURRENT_LIST_DIR}/org.denoming.speechee.conf
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/dbus-1/system.d
    COMPONENT Speechee_Runtime
)