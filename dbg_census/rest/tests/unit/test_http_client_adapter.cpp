#include <gtest/gtest.h>

#include "http_client_adapter.h"

TEST(HttpClientAdapter, SameClientReturnedForSameUrl) {
    dbg_census::rest::HttpClientAdapter adapter;
    const auto client1 = adapter.getClient("http://localhost:8080");
    const auto client2 = adapter.getClient("http://localhost:8080");
    ASSERT_EQ(client1, client2);
}

TEST(HttpClientAdapter, SameClientReturnedForSimilarUrl) {
    dbg_census::rest::HttpClientAdapter adapter;
    const auto client1 = adapter.getClient("http://localhost:8080");
    const auto client2 = adapter.getClient("http://localhost:8080/");
    const auto client3 = adapter.getClient("http://localhost:8080/bogus");
    ASSERT_EQ(client1, client2);
    ASSERT_EQ(client1, client3);
}
