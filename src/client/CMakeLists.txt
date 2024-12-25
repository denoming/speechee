include(GNUInstallDirs)
include(GenerateExportHeader)

add_library(speechee-client SHARED)
add_library(Speechee::Client ALIAS speechee-client)

generate_export_header(speechee-client
    BASE_NAME SPEECHEE
    EXPORT_FILE_NAME speechee/SpeecheeExport.hpp
)

set_target_properties(speechee-client
    PROPERTIES
        EXPORT_NAME Speechee
)

set_target_properties(speechee-client
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
)

target_compile_features(speechee-client PUBLIC cxx_std_23)

target_include_directories(speechee-client
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)

target_sources(speechee-client
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}"
                  "${CMAKE_CURRENT_LIST_DIR}/include"
        FILES "${CMAKE_CURRENT_LIST_DIR}/include/speechee/ISpeakerClient.hpp"
              "${CMAKE_CURRENT_BINARY_DIR}/speechee/SpeecheeExport.hpp"
)

if(ENABLE_HTTP_SUPPORT)
    target_sources(speechee-client
        PUBLIC
            FILE_SET HEADERS
            FILES "${CMAKE_CURRENT_LIST_DIR}/include/speechee/HttpSpeakerClient.hpp"
        PRIVATE
            src/HttpSpeakerClient.cpp
    )
    target_link_libraries(speechee-client
        PRIVATE Boost::headers
                Boost::json
                Jarvisto::Core
                Jarvisto::Network
    )
    target_compile_definitions(speechee-client PRIVATE BOOST_ASIO_NO_DEPRECATED=1)
endif()

if(ENABLE_DBUS_SUPPORT)
    target_sources(speechee-client
        PUBLIC
            FILE_SET HEADERS
            FILES "${CMAKE_CURRENT_LIST_DIR}/include/speechee/DbusSpeakerClient.hpp"
        PRIVATE
            src/DbusSpeakerProxy.cpp
            src/DbusSpeakerClient.cpp
    )
    target_link_libraries(speechee-client PRIVATE Speechee::Dbus)
endif()

install(TARGETS speechee-client
    EXPORT Speechee
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Speechee_Runtime
        NAMELINK_COMPONENT SpeecheeDevelopment
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT SpeecheeDevelopment
    FILE_SET HEADERS
        COMPONENT SpeecheeDevelopment
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT Speechee
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speechee
    NAMESPACE Speechee::
    COMPONENT SpeecheeDevelopment
)
