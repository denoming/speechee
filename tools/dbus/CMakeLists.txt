set(TARGET speechee-dbus)

add_library(${TARGET} INTERFACE)
add_library(Speechee::Dbus ALIAS ${TARGET})

target_include_directories(${TARGET}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${TARGET}
    INTERFACE SDBusCpp::sdbus-c++
)