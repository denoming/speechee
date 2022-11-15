include(FeatureSummary)

option(JARVIS_ENABLE_CLI "Enable building cli" ON)
add_feature_info(
    JARVIS_ENABLE_CLI JARVIS_ENABLE_CLI "Enable building project with cli"
)

option(JARVIS_ENABLE_TESTS "Enable testing" ON)
add_feature_info(
    ENABLE_TESTS JARVIS_ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
