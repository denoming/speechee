include(GNUInstallDirs)

add_executable(speechee-cli "")
add_executable(Speechee::Speaker::Cli ALIAS speechee-cli)

target_sources(speechee-cli
    PRIVATE
        src/main.cpp
)

target_link_libraries(speechee-cli
    PRIVATE cxxopts::cxxopts
            Speechee::Tts
            Speechee::Client
)

target_include_directories(speechee-cli
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_compile_features(speechee-cli PRIVATE cxx_std_23)

install(
    TARGETS speechee-cli
    COMPONENT Speechee_Runtime
)
