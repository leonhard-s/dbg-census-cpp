// Copyright 2023 Leonhard S.

#include "http_client_adapter.h"
#include "url_splitter.h"

namespace dbg_census::rest {

std::shared_ptr<Client> HttpClientAdapter::getClient(const std::string& url) {
    const auto host_with_schema = getHostFromUrl(url, true);
    const auto host = getHostFromUrl(url, false);
    if(m_clients.find(host_with_schema) == m_clients.end()) {
        m_clients[host_with_schema] = std::make_shared<Client>(host);
    }
    return m_clients[host_with_schema];
}

} // namespace dbg_census::rest
