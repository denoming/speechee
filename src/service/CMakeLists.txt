set(TARGET jarvis-speaker-service)

add_library(${TARGET} STATIC)
add_library(Jarvis::Speaker::Service ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE src/SpeakerService.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_link_libraries(${TARGET}
    PRIVATE
        Jarvis::Speaker::DbusGlue
        Jarvis::Speaker::Speech
    PRIVATE
        SDBusCpp::sdbus-c++
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)
