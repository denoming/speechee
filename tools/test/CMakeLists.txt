add_library(speechee-test STATIC)
add_library(Speechee::Test ALIAS speechee-test)

target_sources(speechee-test
    PRIVATE src/TestMain.cpp
)

target_include_directories(speechee-test
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_link_libraries(speechee-test
    PUBLIC GTest::gtest
           GTest::gmock
)

target_compile_features(speechee-test PUBLIC cxx_std_23)
