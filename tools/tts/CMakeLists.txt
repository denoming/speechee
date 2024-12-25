add_library(speechee-tts STATIC)
add_library(Speechee::Tts ALIAS speechee-tts)

target_include_directories(speechee-tts
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(speechee-tts
    PRIVATE Boost::headers
)

if(ENABLE_ONLINE_TTS)
    target_link_libraries(speechee-tts PRIVATE google-cloud-cpp::texttospeech)
    target_sources(speechee-tts PRIVATE src/OnlineTextToSpeechClient.cpp)
endif()
if(ENABLE_OFFLINE_TTS)
    target_link_libraries(speechee-tts PRIVATE Voxer::Voxer)
    target_sources(speechee-tts PRIVATE src/OfflineTextToSpeechEngine.cpp)
endif()

target_compile_features(speechee-tts PUBLIC cxx_std_23)
