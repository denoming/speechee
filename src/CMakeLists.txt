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
        Jarvis::Speaker::Speaker
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT JarvisSpeakerRuntime
)

add_subdirectory(client)
add_subdirectory(speaker)
