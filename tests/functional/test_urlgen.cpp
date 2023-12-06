#include <gtest/gtest.h>
#include "dbg_census/urlgen/url_builder.h"

TEST(ExampleQueries, GetEverquest2Collections) {
    const dbg_census::urlgen::UrlBuilder builder("eq2", "", "s:example");
    auto url = builder.build();
    EXPECT_EQ(url, "https://census.daybreakgames.com/s:example/get/eq2");
}

TEST(ExampleQueries, GetPs2CharacterByName) {
    dbg_census::urlgen::UrlBuilder builder("ps2", "character", "s:example");
    builder.addQueryTerm("name.first_lower", "auroram");
    auto url = builder.build();
    EXPECT_EQ(url, "https://census.daybreakgames.com/s:example/get/ps2/character?name.first_lower=auroram");
}

TEST(ExampleQueries, GetFacilityTypesFromSanctuary) {
    dbg_census::urlgen::UrlBuilder builder("ps2", "facility_type", "s:example");
    builder.setBaseUrl("https://census.lithafalcon.cc");
    builder.addQueryCommand("c:limit=100");
    auto url = builder.build();
    EXPECT_EQ(url, "https://census.lithafalcon.cc/s:example/get/ps2/facility_type?c:limit=100");
}
