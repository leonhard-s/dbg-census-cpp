// Copyright 2023 Leonhard S.

#pragma once

#include <map>
#include <memory>
#include <string>

#include <ixwebsocket/IXHttpClient.h>

using Client = ix::HttpClient;

namespace dbg_census::rest {

class HttpClientAdapter {
public:
    HttpClientAdapter() = default;

    virtual ~HttpClientAdapter() = default;
    HttpClientAdapter(const HttpClientAdapter&) = delete;
    HttpClientAdapter(HttpClientAdapter&&) noexcept = default;
    HttpClientAdapter& operator=(const HttpClientAdapter&) = delete;
    HttpClientAdapter& operator=(HttpClientAdapter&&) noexcept = default;

    /**
     * @brief Return a client for a given base URL.
     * The httplib client is initialized with the base URL, which can
     * not be changed afterwards. To hide this from the client code, we
     * create one client per base URL (usually only one) and reuse it
     * whenever we make a request to that server.
     * @param url The base URL for the client.
     * @return A client for the given base URL.
    */
    std::shared_ptr<Client> getClient(const std::string& url);

private:
    std::map<std::string, std::shared_ptr<Client>> m_clients;
};

} // namespace dbg_census::rest
