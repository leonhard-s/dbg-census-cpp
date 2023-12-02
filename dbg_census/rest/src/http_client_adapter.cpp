// Copyright 2023 Leonhard S.

#include "http_client_adapter.h"
#include <ixwebsocket/IXNetSystem.h>
#include "url_splitter.h"

namespace dbg_census::rest {


auto HttpClientAdapter::getClient() -> ix::HttpClient* {
    if (!m_client) {
        ix::initNetSystem();
        m_client = std::make_unique<ix::HttpClient>();
    }
    return m_client.get();
}

} // namespace dbg_census::rest
