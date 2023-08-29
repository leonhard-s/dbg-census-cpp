#include <gtest/gtest.h>

#include "dbg_census/stream/subscription_builder.h"

TEST(SubscriptionBuilder, PlayerLogging) {
    dbg_census::stream::SubscriptionBuilder builder;
    builder.setEventNames({ "PlayerLogin", "PlayerLogout" });
    EXPECT_NE(builder.build().find(R"("eventNames":["PlayerLogin","PlayerLogout"])"),
        std::string::npos);
}

TEST(SubscriptionBuilder, LogicalAnd) {
    dbg_census::stream::SubscriptionBuilder builder("Death");
    builder.setCharacters({}, true);
    builder.setWorlds({1});
    builder.setLogicalAndCharactersWithWorlds(true);
    EXPECT_EQ(builder.build(),
        R"({"service":"event","action":"subscribe","eventNames":["Death"],"characters":["all"],"worlds":["1"],"logicalAndCharactersWithWorlds":true})");
}
