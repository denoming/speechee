add_library(speechee-dbus INTERFACE)
add_library(Speechee::Dbus ALIAS speechee-dbus)

target_include_directories(speechee-dbus
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(speechee-dbus
    INTERFACE SDBusCpp::sdbus-c++
)