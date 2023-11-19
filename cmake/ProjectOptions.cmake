include(FeatureSummary)

option(ENABLE_DLT "Enable DLT logging" OFF)
add_feature_info(
    ENABLE_DLT ENABLE_DLT "Build project with DLT logging"
)

option(ENABLE_CLI "Enable CLI building" OFF)
if(ENABLE_CLI)
    list(APPEND VCPKG_MANIFEST_FEATURES "cli")
endif()
add_feature_info(
    ENABLE_CLI ENABLE_CLI "Enable building project with CLI"
)

option(ENABLE_TESTS "Enable testing" OFF)
if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

option(ENABLE_DBUS_SUPPORT "Enable DBus support" OFF)
add_feature_info(
    ENABLE_CLI ENABLE_CLI "Enable DBus support (client+service)"
)

option(ENABLE_HTTP_SUPPORT "Enable HTTP support" ON)
if(ENABLE_CLI)
    list(APPEND VCPKG_MANIFEST_FEATURES "http")
endif()
add_feature_info(
    ENABLE_CLI ENABLE_CLI "Enable HTTP support (service)"
)

option(ENABLE_INTEGRATION "Enable HomeAssistant integration support" OFF)
add_feature_info(
    ENABLE_INTEGRATION ENABLE_INTEGRATION "Enable HomeAssistant integration support"
)

feature_summary(WHAT ALL)
