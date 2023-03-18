set(TARGET jarvis-speaker-dbus)

add_library(${TARGET} INTERFACE)
add_library(Jarvis::Speaker::Dbus ALIAS ${TARGET})

target_include_directories(${TARGET}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${TARGET}
    INTERFACE SDBusCpp::sdbus-c++
)