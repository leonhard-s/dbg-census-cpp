#include <gtest/gtest.h>

#include "http_client_adapter.h"

TEST(HttpClientAdapter, OnlyOneClientCreated) {
    dbg_census::rest::HttpClientAdapter adapter;
    const auto client1 = adapter.getClient();
    const auto client2 = adapter.getClient();
    ASSERT_EQ(client1, client2);
}
