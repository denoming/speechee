list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

if (EXISTS "$ENV{HOME}/.local")
    list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/.local)
endif()

if (DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Toolchain file ${CMAKE_TOOLCHAIN_FILE}")
endif()

include(BuildType)
include(BuildLocation)
include(BuildOptions)

if (ENABLE_TESTS)
    enable_testing()
    include(AddGoogleTest)
    message(VERBOSE "Building with tests")
else()
    message(VERBOSE "Building without tests")
endif()

if(NOT SYSTEMD_UNAME)
    set(SYSTEMD_UNAME "speechee" CACHE STRING "User to run service as" FORCE)
endif()
message(VERBOSE "Systemd user: ${SYSTEMD_UNAME}")
if(NOT SYSTEMD_GNAME)
    set(SYSTEMD_GNAME "speechee" CACHE STRING "Group to run service as" FORCE)
endif()
message(VERBOSE "Systemd group: ${SYSTEMD_GNAME}")

include(AddGstLite)
if(ENABLE_HTTP_SUPPORT OR ENABLE_CLI)
    include(AddBoost)
endif()
include(AddSigCpp)
include(AddGLibmm)
include(AddGst)
include(AddJarvisto)
if(ENABLE_ONLINE_TTS)
    include(AddGoogleTts)
endif()
if(ENABLE_DBUS_SUPPORT)
    include(AddSdbusCpp)
endif()
if(ENABLE_OFFLINE_TTS)
    include(AddVoxer)
endif()
