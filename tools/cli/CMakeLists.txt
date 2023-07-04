include(GNUInstallDirs)

set(TARGET speechee-cli)

add_executable(${TARGET} "")
add_executable(Speechee::Speaker::Cli ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE
        src/main.cpp
)

target_link_libraries(${TARGET}
    PRIVATE Boost::headers
            Boost::program_options
    PRIVATE Speechee::Client
)

target_compile_features(${TARGET} PRIVATE cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT SpeecheeRuntime
)
