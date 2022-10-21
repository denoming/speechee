set(TARGET jarvis-speaker-dbus)

find_package(sdbus-c++-tools REQUIRED)

set(DBUS_CLIENT_GLUE_FILE jarvis-speaker-client-glue.h)
set(DBUS_SERVER_GLUE_FILE jarvis-speaker-server-glue.h)

add_custom_command(
    OUTPUT ${DBUS_CLIENT_GLUE_FILE} ${DBUS_SERVER_GLUE_FILE}
    COMMAND SDBusCpp::sdbus-c++-xml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/org.denoming.jarvis.speaker.xml
            --proxy=${DBUS_CLIENT_GLUE_FILE} --adaptor=${DBUS_SERVER_GLUE_FILE}
    DEPENDS org.denoming.jarvis.speaker.xml
    COMMENT "Generating D-Bus interfaces for ${PROJECT_NAME}."
    VERBATIM
)

add_library(${TARGET} INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_CLIENT_GLUE_FILE}
    ${CMAKE_CURRENT_BINARY_DIR}/${DBUS_SERVER_GLUE_FILE}
)
add_library(Jarvis::Speaker::DbusGlue ALIAS ${TARGET})

target_include_directories(${TARGET}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)