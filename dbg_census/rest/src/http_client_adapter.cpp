// Copyright 2023 Leonhard S.

#include "http_client_adapter.h"
#include <ixwebsocket/IXNetSystem.h>
#include "url_splitter.h"

namespace dbg_census::rest {

auto HttpClientAdapter::getClient() -> std::shared_ptr<ix::HttpClient> {
    if (!m_client) {
        ix::initNetSystem();
        m_client = std::make_shared<ix::HttpClient>();
    }
    return m_client;
}

} // namespace dbg_census::rest
