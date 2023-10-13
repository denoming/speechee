#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "speaker/GeneralConfig.hpp"

using namespace jar;

static const std::string_view kConfigStr = R"(
synthesis =
{
    threads = 2;
};

mqtt =
{
    user = "test";
    password = "123456";
    server = "192.168.1.2";
};

services =
{
    http =
    {
        port = 8080;
        threads = 2;
    };
};
)";

TEST(GeneralConfigTest, Load)
{
    GeneralConfig config;
    ASSERT_TRUE(config.load(kConfigStr));

    EXPECT_EQ(config.mqttUser(), "test");
    EXPECT_EQ(config.mqttPassword(), "123456");
    EXPECT_EQ(config.mqttServer(), "192.168.1.2");

    EXPECT_EQ(config.synthesisThreads(), 2);

    EXPECT_EQ(config.httpServicePort(), 8080);
    EXPECT_EQ(config.httpServiceThreads(), 2);
}