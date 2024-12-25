set(TARGET speechee-speaker)

add_library(${TARGET} STATIC)
add_library(Speechee::Speaker ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
            $<BUILD_INTERFACE:${VOXER_STAGE_DIR}>
)

target_link_libraries(${TARGET}
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

target_sources(${TARGET}
    PRIVATE src/Types.cpp
            src/SpeechSynthesizePool.cpp
            src/SpeechSynthesizeTask.cpp
            src/AudioBufferList.cpp
            src/Player.cpp
            src/PlayerLoop.cpp
            src/PlayerFactory.cpp
            src/Speaker.cpp
            src/SpeakerSubsystem.cpp
            src/ConfigLoader.cpp
            src/GeneralConfig.cpp
            src/GstInitializer.cpp
)

if(ENABLE_DBUS_SUPPORT)
    target_link_libraries(${TARGET}
        PRIVATE Speechee::Dbus
    )
    target_sources(${TARGET}
        PRIVATE src/SpeakerAdapter.cpp
                src/DbusSpeakerService.cpp
    )
endif()

if(ENABLE_HTTP_SUPPORT)
    target_sources(${TARGET}
        PRIVATE src/HttpSpeakerService.cpp
    )
    target_compile_definitions(${TARGET} PRIVATE BOOST_ASIO_NO_DEPRECATED=1)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_23)
