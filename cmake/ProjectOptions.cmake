include(FeatureSummary)

option(ENABLE_DLT "Enable DLT logging" OFF)
add_feature_info(
    ENABLE_DLT ENABLE_DLT "Build project with DLT logging"
)

option(ENABLE_CLI "Enable building cli" ON)
add_feature_info(
    ENABLE_CLI ENABLE_CLI "Enable building project with cli"
)

option(ENABLE_TESTS "Enable testing" ON)
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

option(ENABLE_INTEGRATION "Enable HomeAssistant integration" OFF)
add_feature_info(
    ENABLE_INTEGRATION ENABLE_INTEGRATION "Build project with HomeAssistant integration"
)

feature_summary(WHAT ALL)
