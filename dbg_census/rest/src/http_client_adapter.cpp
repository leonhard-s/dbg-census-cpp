// Copyright 2023 Leonhard S.

#include "http_client_adapter.h"

#include <memory>

#include <ixwebsocket/IXHttpClient.h>
#include <ixwebsocket/IXNetSystem.h>

namespace dbg_census::rest {


ix::HttpClient* HttpClientAdapter::getClient() {
    if(!m_client) {
        ix::initNetSystem();
        m_client = std::make_unique<ix::HttpClient>();
    }
    return m_client.get();
}

} // namespace dbg_census::rest
