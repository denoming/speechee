include(GNUInstallDirs)

install(
    FILES "${CMAKE_CURRENT_LIST_DIR}/default.json"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/speechee/config"
    COMPONENT Speechee_Runtime
)