// Copyright 2023 Leonhard S.

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "dbg_census/stream/export.h"
#include "dbg_census/common/macros.h"

namespace dbg_census::stream {

namespace detail {

DBGCENSUS_API std::string_view defaultEssEndpoint();

DBGCENSUS_API std::string_view defaultEssEnvironment();

DBGCENSUS_API std::string_view defaultEssServiceId();

} // namespace detail

class DBGCENSUS_API EssClient {
public:
    explicit EssClient(
        std::string_view ess_service_id = detail::defaultEssServiceId(),
        std::string_view ess_environment = detail::defaultEssEnvironment(),
        std::string_view ess_endpoint = detail::defaultEssEndpoint()
    );
    virtual ~EssClient();
    EssClient(const EssClient&) = delete;
    EssClient& operator=(const EssClient&) = delete;
    EssClient(EssClient&&) = default;
    EssClient& operator=(EssClient&&) = default;

    void connect();
    void disconnect();

    void setOnConnectCallback(std::function<void()> callback);
    void setOnDisconnectCallback(std::function<void()> callback);
    void setOnReadyCallback(std::function<void()> callback);
    void setOnMessageCallback(std::function<void(const std::string&)> callback);
    void setOnEventPayloadCallback(std::function<void(const std::string&)> callback);

    void send(const std::string& message);

private:
    class Impl;
    DISABLE_WARNING_4251;
    std::unique_ptr<Impl> m_impl;
    ENABLE_WARNING_4251;
};

} // namespace dbg_census::stream
