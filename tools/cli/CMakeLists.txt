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
            Speechee::Tts
            Speechee::Client
)

target_include_directories(${TARGET}
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_compile_features(${TARGET} PRIVATE cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT SpeecheeRuntime
)
