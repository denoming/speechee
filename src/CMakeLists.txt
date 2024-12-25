include(GNUInstallDirs)

add_executable(speechee "")
add_executable(Speechee::Speechee ALIAS speechee)

target_sources(speechee
    PRIVATE ServiceApplication.cpp
)

target_link_libraries(speechee
    PRIVATE
        Jarvisto::Core
        Speechee::Service
)

target_include_directories(speechee
    PRIVATE $<BUILD_INTERFACE:${SPEECHEE_STAGE_DIR}>
)

target_compile_features(speechee PUBLIC cxx_std_23)

install(
    TARGETS speechee
    COMPONENT Speechee_Runtime
)

if(ENABLE_DBUS_SUPPORT OR ENABLE_HTTP_SUPPORT)
    add_subdirectory(client)
endif()
add_subdirectory(service)
