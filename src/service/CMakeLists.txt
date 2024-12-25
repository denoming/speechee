add_library(speechee-service STATIC)
add_library(Speechee::Service ALIAS speechee-service)

target_include_directories(speechee-service
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${SPEECHEE_STAGE_DIR}>
)

target_link_libraries(speechee-service
    PUBLIC Jarvisto::Core
           Jarvisto::Network
           PkgConfig::SigCpp
           PkgConfig::GLibmm
    PRIVATE Boost::headers
            Boost::json
            PkgConfig::Gst
            PkgConfig::GstApp
            PkgConfig::LibConfig
            Speechee::Tts
)

target_sources(speechee-service
    PRIVATE src/SpeechSynthesizePool.cpp
            src/SpeechSynthesizeTask.cpp
            src/AudioBufferList.cpp
            src/Player.cpp
            src/PlayerLoop.cpp
            src/PlayerFactory.cpp
            src/Speaker.cpp
            src/SpeakerSubsystem.cpp
            src/GstInitializer.cpp
)

if(ENABLE_DBUS_SUPPORT)
    target_link_libraries(speechee-service
        PRIVATE Speechee::Dbus
    )
    target_sources(speechee-service
        PRIVATE src/SpeakerAdapter.cpp
                src/DbusSpeakerService.cpp
    )
endif()

if(ENABLE_HTTP_SUPPORT)
    target_sources(speechee-service
        PRIVATE src/HttpSpeakerService.cpp
    )
    target_compile_definitions(speechee-service PRIVATE BOOST_ASIO_NO_DEPRECATED=1)
endif()

target_compile_features(speechee-service PUBLIC cxx_std_23)
