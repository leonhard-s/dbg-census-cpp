// Copyright 2023 Leonhard S.

#include "http_client_adapter.h"
#include <ixwebsocket/IXNetSystem.h>
#include "url_splitter.h"

namespace dbg_census::rest {

std::shared_ptr<ix::HttpClient> HttpClientAdapter::getClient() {
    if(!m_initialized) {
        ix::initNetSystem();
        m_client = std::make_shared<ix::HttpClient>();
        m_initialized = true;
    }
    return m_client;
}

} // namespace dbg_census::rest
