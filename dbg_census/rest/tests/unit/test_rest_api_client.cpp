#include <gtest/gtest.h>

#include "dbg_census/rest/rest_api_client.h"
#include "dbg_census/rest/retry_strategy.h"

namespace {

class DummyRestApiClient final : public dbg_census::rest::RestApiClient {
public:
    std::size_t m_query_attempts = 0;

    using RestApiClient::RestApiClient;
    ~DummyRestApiClient() override = default;
    DummyRestApiClient(DummyRestApiClient&&) = delete;
    DummyRestApiClient& operator=(DummyRestApiClient&&) = delete;
    DummyRestApiClient(const DummyRestApiClient&) = delete;
    DummyRestApiClient& operator=(const DummyRestApiClient&) = delete;

private:
    std::string runQuery(const std::string& query) override {
        m_query_attempts++;
        if(query == "always_fail") {
            throw std::runtime_error("always_fail");
        }
        return "payload_ok";
    }
};

} // anonymous namespace

TEST(RestApiClient, QueryIsRerunOnRetry) {
    DummyRestApiClient client(std::make_unique<dbg_census::rest::RetryOnce>());
    auto result = client.request("always_fail");
    ASSERT_EQ(client.m_query_attempts, 2);
}

TEST(RestApiClient, QueryIsNotRerunOnSuccess) {
    DummyRestApiClient client(std::make_unique<dbg_census::rest::RetryOnce>());
    auto result = client.request("always_succeed");
    ASSERT_EQ(client.m_query_attempts, 1);
}

TEST(RestApiClient, DefaultRetryPolicyDoesNotRetry) {
    DummyRestApiClient client;
    auto result = client.request("always_fail");
    ASSERT_FALSE(result.has_value());
    ASSERT_EQ(client.m_query_attempts, 1);
}
