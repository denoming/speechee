include(GNUInstallDirs)

set(TARGET jarvis-speaker)

add_executable(${TARGET} "")
add_executable(Jarvis::Speaker ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE
        common/LoggerInitializer.cpp
        SpeakerService.cpp
)

target_include_directories(${TARGET}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_link_libraries(${TARGET}
    PRIVATE
        Boost::headers
        Boost::program_options
        Boost::system
    PRIVATE
        spdlog::spdlog
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

install(
    TARGETS ${TARGET}
    COMPONENT JarvisSpeaker_Runtime
)
