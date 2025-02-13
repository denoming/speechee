include(FeatureSummary)
include(CMakeDependentOption)

option(ENABLE_TESTS "Enable testing" OFF)
if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

option(ENABLE_DBUS_SUPPORT "Enable DBus support" OFF)
if(ENABLE_DBUS_SUPPORT)
    list(APPEND VCPKG_MANIFEST_FEATURES "dbus")
endif()
add_feature_info(
    ENABLE_DBUS_SUPPORT ENABLE_DBUS_SUPPORT "Enable DBus support (client+service)"
)

option(ENABLE_HTTP_SUPPORT "Enable HTTP support" ON)
if(ENABLE_HTTP_SUPPORT)
    list(APPEND VCPKG_MANIFEST_FEATURES "http")
endif()
add_feature_info(
    ENABLE_HTTP_SUPPORT ENABLE_HTTP_SUPPORT "Enable HTTP support (service)"
)

cmake_dependent_option(ENABLE_CLI "Enable CLI building" ON "ENABLE_DBUS_SUPPORT OR ENABLE_HTTP_SUPPORT" OFF)
if(ENABLE_CLI)
    list(APPEND VCPKG_MANIFEST_FEATURES "cli")
endif()
add_feature_info(
    ENABLE_CLI ENABLE_CLI "Enable building project with CLI"
)

option(ENABLE_INTEGRATION "Enable HomeAssistant integration support" OFF)
add_feature_info(
    ENABLE_INTEGRATION ENABLE_INTEGRATION "Enable HomeAssistant integration support"
)

option(ENABLE_ONLINE_TTS "Enable online TTS engine support" OFF)
if(ENABLE_ONLINE_TTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "online")
endif()
add_feature_info(
    ENABLE_ONLINE_TTS ENABLE_ONLINE_TTS "Enable online TTS engine support"
)

option(ENABLE_OFFLINE_TTS "Enable offline TTS engine support" ON)
if(ENABLE_OFFLINE_TTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "offline")
endif()
add_feature_info(
    ENABLE_OFFLINE_TTS ENABLE_OFFLINE_TTS "Enable offline TTS engine support"
)

if(ENABLE_ONLINE_TTS AND ENABLE_OFFLINE_TTS)
    message(FATAL_ERROR "Only one TTS engine at a time can be enabled")
endif()
if(NOT ENABLE_ONLINE_TTS AND NOT ENABLE_OFFLINE_TTS)
    message(FATAL_ERROR "At least one one TTS engine should be enabled")
endif()

feature_summary(WHAT ALL)
