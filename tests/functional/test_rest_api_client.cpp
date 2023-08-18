#include <memory>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "dbg_census/urlgen/url_builder.h"
#include "dbg_census/rest/rest_api_client.h"
#include "dbg_census/rest/retry_strategy.h"

TEST(RunQueries, GetPs2NamespaceContents) {
    auto client = dbg_census::rest::RestApiClient();
    auto url = dbg_census::urlgen::UrlBuilder("ps2").build();
    auto result = client.request(url);
    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result.value().empty());
    nlohmann::json json;
    ASSERT_NO_THROW(json = nlohmann::json::parse(result.value()));
    ASSERT_TRUE(json.is_object());
    ASSERT_TRUE(json.contains("datatype_list"));
}
