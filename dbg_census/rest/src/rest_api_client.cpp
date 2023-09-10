// Copyright 2023 Leonhard S.

#include "dbg_census/rest/rest_api_client.h"
#include <chrono>
#include <ixwebsocket/IXHttp.h>
#include "dbg_census/rest/retry_strategy.h"
#include "http_client_adapter.h"
#include "url_splitter.h"

namespace dbg_census::rest {

RestApiClient::RestApiClient()
    : m_retry_strategy{ std::make_unique<NoRetry>() }
    , m_http_client_adapter{ std::make_unique<HttpClientAdapter>() }
{}

RestApiClient::RestApiClient(std::unique_ptr<RetryStrategy> retryStrategy)
    : m_retry_strategy{ std::move(retryStrategy) }
    , m_http_client_adapter{ std::make_unique<HttpClientAdapter>() }
{}

RestApiClient::~RestApiClient() = default;

std::optional<std::string> RestApiClient::request(const std::string& query) {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::size_t attempts = 0;
    std::chrono::milliseconds time_taken;
    while(true) {
        time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        try {
            return runQuery(query);
        }
        catch([[maybe_unused]] const std::exception& e) {
            if(!m_retry_strategy->shouldRetry(600, attempts++, time_taken)) {
                return std::nullopt;
            }
            std::this_thread::sleep_for(m_retry_strategy->getRetryDelay(attempts));
        }
    }
}

std::string RestApiClient::runQuery([[maybe_unused]] const std::string& query) {
    auto client = m_http_client_adapter->getClient(query);

    auto args = client->createRequest();
    args->extraHeaders["Accept"] = "application/json";
    args->compress = true;

    auto response = client->get(query, args);
    if(response == nullptr) {
        throw std::runtime_error("HTTP request failed");
    }
    if(response->statusCode != 200) {
        throw std::runtime_error("HTTP request failed with status code " + std::to_string(response->statusCode));
    }
    const auto payload = response->body;
    if(!validatePayload(payload)) {
        throw std::runtime_error("Payload validation failed");
    }
    return payload;
}

} // namespace dbg_census::rest
