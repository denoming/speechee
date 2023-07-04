include(FeatureSummary)

option(ENABLE_CLI "Enable building cli" ON)
add_feature_info(
    ENABLE_CLI ENABLE_CLI "Enable building project with cli"
)

option(ENABLE_TESTS "Enable testing" ON)
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
