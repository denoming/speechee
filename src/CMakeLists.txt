include(GNUInstallDirs)

set(TARGET speechee)

add_executable(${TARGET} "")
add_executable(Speechee::Speechee ALIAS ${TARGET})

target_sources(${TARGET}
    PRIVATE ServiceApplication.cpp
)

target_link_libraries(${TARGET}
    PRIVATE
        Jarvisto::Jarvisto
        Speechee::Speaker
)

target_include_directories(${TARGET}
    PRIVATE $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/options>
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT SpeecheeRuntime
)

add_subdirectory(client)
add_subdirectory(speaker)
