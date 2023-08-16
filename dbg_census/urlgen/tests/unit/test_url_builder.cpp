#include <string>
#include <gtest/gtest.h>

#include "dbg_census/urlgen/url_builder.h"

TEST(UrlBuilder, DefaultBaseUrl) {
    auto url = dbg_census::urlgen::UrlBuilder("bogus").build();
    EXPECT_TRUE(url.starts_with("https://census.daybreakgames.com"));
}

TEST(UrlBuilder, DefaultServiceId) {
    auto url = dbg_census::urlgen::UrlBuilder("bogus").build();
    auto pos = url.find("/s:example/", 0);
    EXPECT_NE(pos, std::string::npos);
}

TEST(UrlBuilder, DefaultQueryVerb) {
    auto url = dbg_census::urlgen::UrlBuilder("bogus").build();
    auto pos = url.find("/get/", 0);
    EXPECT_NE(pos, std::string::npos);
}

TEST(UrlBuilder, CanOverrideBaseUrl) {
    const std::string sanctuary_census = "https://census.lithafalcon.cc/";
    auto builder = dbg_census::urlgen::UrlBuilder("bogus");
    builder.setBaseUrl(sanctuary_census);
    auto url = builder.build();
    EXPECT_TRUE(url.starts_with(sanctuary_census));
}

TEST(UrlBuilder, CanOverrideServiceId) {
    const std::string service_id = "s:example2";
    auto builder = dbg_census::urlgen::UrlBuilder("bogus");
    builder.setServiceId(service_id);
    auto url = builder.build();
    auto pos = url.find("/" + service_id + "/", 0);
    EXPECT_NE(pos, std::string::npos);
}

TEST(UrlBuilder, CanOverrideQueryVerb) {
    const std::string query_verb = "count";
    auto builder = dbg_census::urlgen::UrlBuilder("bogus");
    builder.setQueryVerb(query_verb);
    auto url = builder.build();
    auto pos = url.find("/" + query_verb + "/", 0);
    EXPECT_NE(pos, std::string::npos);
}

TEST(UrlBuilder, CanSetNamespace) {
    const std::string ns = "ps2";
    auto builder = dbg_census::urlgen::UrlBuilder();
    builder.setNamespace(ns);
    builder.setCollection("bogus");
    auto url = builder.build();
    auto pos = url.find("/" + ns + "/", 0);
    EXPECT_NE(pos, std::string::npos);
}

TEST(UrlBuilder, CanSetCollection) {
    const std::string collection = "character";
    auto builder = dbg_census::urlgen::UrlBuilder("bogus");
    builder.setCollection(collection);
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("/" + collection));
}

TEST(UrlBuilder, CollectionSkippedIfEmpty) {
    auto builder = dbg_census::urlgen::UrlBuilder("bogus");
    builder.setCollection("");
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("/bogus"));
}

TEST(UrlBuilder, CollectionEmptyByDefault) {
    auto builder = dbg_census::urlgen::UrlBuilder("bogus");
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("/bogus"));
}

TEST(UrlBuilder, ServiceIdPrefixIsAppendedIfMissing) {
    // If set via constructor
    auto builder = dbg_census::urlgen::UrlBuilder("bogus", "bogus", "example");
    auto url = builder.build();
    auto pos = url.find("/s:example/", 0);
    EXPECT_NE(pos, std::string::npos);

    // If set via setter
    builder = dbg_census::urlgen::UrlBuilder("bogus");
    builder.setServiceId("example");
    url = builder.build();
    pos = url.find("/s:example/", 0);
    EXPECT_NE(pos, std::string::npos);
}

TEST(UrlBuilder, CanAddSingleQueryItem) {
    auto builder = dbg_census::urlgen::UrlBuilder("ps2", "character");
    builder.addQueryTerm("name.first_lower", "higby");
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("?name.first_lower=higby"));
}

TEST(UrlBuilder, CanAddMultipleQueryItems) {
    auto builder = dbg_census::urlgen::UrlBuilder("ps2", "character");
    builder.addQueryTerms({
        {"name.first", "^Hig"},
        {"name.first_lower", "by"}
        });
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("?name.first=^Hig&name.first_lower=by"));
}

TEST(UrlBuilder, CanAddSingleQueryCommand) {
    auto builder = dbg_census::urlgen::UrlBuilder("ps2", "character");
    builder.addQueryCommand("limit=10");
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("?c:limit=10"));
}

TEST(UrlBuilder, CanAddMultipleQueryCommands) {
    auto builder = dbg_census::urlgen::UrlBuilder("ps2", "character");
    builder.addQueryCommands({
        "limit=10",
        "sort=times.date_created:-1"
        });
    auto url = builder.build();
    EXPECT_TRUE(url.ends_with("?c:limit=10&c:sort=times.date_created:-1"));
}

TEST(UrlBuilder, PathOrderIsValid) {
    auto builder = dbg_census::urlgen::UrlBuilder("namespace", "collection", "service_id");
    builder.setBaseUrl("base_url");
    builder.setQueryVerb("verb");
    builder.addQueryTerm("term", "value");
    builder.addQueryCommand("command=1");
    auto url = builder.build();
    EXPECT_EQ(url, "base_url/s:service_id/verb/namespace/collection?term=value&c:command=1");
}
