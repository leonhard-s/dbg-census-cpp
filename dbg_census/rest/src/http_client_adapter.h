// Copyright 2023 Leonhard S.

#pragma once

#include <map>
#include <memory>
#include <string>
#include <ixwebsocket/IXHttpClient.h>

namespace dbg_census::rest {

class HttpClientAdapter {
public:
    HttpClientAdapter() = default;

    virtual ~HttpClientAdapter() = default;
    HttpClientAdapter(const HttpClientAdapter&) = delete;
    HttpClientAdapter(HttpClientAdapter&&) noexcept = default;
    HttpClientAdapter& operator=(const HttpClientAdapter&) = delete;
    HttpClientAdapter& operator=(HttpClientAdapter&&) noexcept = default;

    std::shared_ptr<ix::HttpClient> getClient();

private:
    std::shared_ptr<ix::HttpClient> m_client;
    bool m_initialized = false;
};

} // namespace dbg_census::rest
