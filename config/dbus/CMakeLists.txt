include(GNUInstallDirs)

install(FILES ${CMAKE_CURRENT_LIST_DIR}/org.denoming.jarvis.speaker.conf
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/dbus-1/system.d
    COMPONENT JarvisSpeakerRuntime
)