// Copyright 2023 Leonhard S.

#pragma once

#include <memory>
#include <optional>
#include <string>
#include "dbg_census/rest/export.h"

namespace dbg_census::rest {

class RetryStrategy;
class HttpClientAdapter;

class RestApiClient {
public:
    DBGCENSUS_API RestApiClient();
    DBGCENSUS_API explicit RestApiClient(std::unique_ptr<RetryStrategy> retryStrategy);

    DBGCENSUS_API virtual ~RestApiClient();
    RestApiClient(const RestApiClient&) = delete;
    DBGCENSUS_API RestApiClient(RestApiClient&&) noexcept = default;
    RestApiClient& operator=(const RestApiClient&) = delete;
    DBGCENSUS_API RestApiClient& operator=(RestApiClient&&) noexcept = default;

    /**
     * @brief Send a request to the given URL.
     * The URL is not validated in any way. Consider using the
     * `UrlBuilder` class to assemble the URL and query parameters.
     * This function contains retry logic according to the `RetryPolicy`
     * selected for the client.
     * @param query The URL to send the request to.
     * @return The response payload, if the request was successful.
    */
    DBGCENSUS_API std::optional<std::string> request(const std::string& query);

protected:
    /**
     * @brief Hook to validate a received payload.
     * This is called after the HTTP request has been validated, but
     * before the request was flagged as successful. If this function
     * returns false or throws any exception, the request will be
     * retried according to the `RetryPolicy` of the client.
     * The default implementation always returns true.
     * @param payload The payload to validate.
     * @return true if the payload is valid, false otherwise.
    */
    DBGCENSUS_API virtual bool validatePayload([[maybe_unused]] const std::string& payload) const { return true; };

private:
    std::unique_ptr<RetryStrategy> m_retry_strategy;
    std::unique_ptr<HttpClientAdapter> m_http_client_adapter;

    DBGCENSUS_API virtual std::string runQuery(const std::string& query);
};

} // namespace dbg_census::rest
