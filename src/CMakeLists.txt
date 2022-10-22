include(GNUInstallDirs)

set(TARGET jarvis-speaker)

add_executable(${TARGET} "")
add_executable(Jarvis::Speaker ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE SpeakerApplication.cpp
)

target_link_libraries(${TARGET}
    PRIVATE
        Jarvis::Common
        Jarvis::Speaker::Speech
    PRIVATE
        Boost::headers
        Boost::program_options
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

install(
    TARGETS ${TARGET}
    COMPONENT JarvisSpeaker_Runtime
)

add_subdirectory(speech)
add_subdirectory(service)