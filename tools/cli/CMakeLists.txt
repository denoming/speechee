include(GNUInstallDirs)

set(TARGET jarvis-speaker-cli)

add_executable(${TARGET} "")
add_executable(Jarvis::Speaker::Cli ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE
        src/main.cpp
)

target_link_libraries(${TARGET}
    PRIVATE Boost::headers Boost::program_options
    PRIVATE Jarvis::Speaker::Client
)

target_compile_features(${TARGET} PRIVATE cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT JarvisSpeakerRuntime
)
