// Copyright 2023 Leonhard S.

#include "http_client_adapter.h"
#include <ixwebsocket/IXNetSystem.h>
#include "url_splitter.h"

namespace dbg_census::rest {

std::shared_ptr<Client> HttpClientAdapter::getClient([[maybe_unused]] const std::string& url) {
    if(m_clients.empty())
        ix::initNetSystem();
        m_clients.emplace("", std::make_shared<Client>());
    return m_clients[""];
}

} // namespace dbg_census::rest
