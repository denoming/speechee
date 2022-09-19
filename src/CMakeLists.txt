include(GNUInstallDirs)

set(TARGET jarvis-speaker)

add_executable(${TARGET} "")
add_executable(Jarvis::Speaker ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE
        main.cpp
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

install(
    TARGETS ${TARGET}
    COMPONENT JarvisSpeaker_Runtime
)
