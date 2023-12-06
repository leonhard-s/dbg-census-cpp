#include <gtest/gtest.h>

#include "http_client_adapter.h"

TEST(HttpClientAdapter, OnlyOneClientCreated) {
    dbg_census::rest::HttpClientAdapter adapter;
    auto* const client1 = adapter.getClient();
    auto* const client2 = adapter.getClient();
    ASSERT_EQ(client1, client2);
}
