// Copyright 2023 Leonhard S.

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "dbg_census/stream/export.h"

namespace dbg_census::stream {

namespace detail {

static const std::string default_ess_endpoint = "wss://push.planetside2.com/streaming";
static const std::string default_ess_environment = "ps2";
static const std::string default_ess_service_id = "s:example";

} // namespace detail

class EssClient {
public:
    DBGCENSUS_API explicit EssClient(
        const std::string& ess_service_id = detail::default_ess_service_id,
        const std::string& ess_environment = detail::default_ess_environment,
        const std::string& ess_endpoint = detail::default_ess_endpoint
    );
    DBGCENSUS_API virtual ~EssClient();
    EssClient(const EssClient&) = delete;
    EssClient& operator=(const EssClient&) = delete;
    DBGCENSUS_API EssClient(EssClient&&) = default;
    DBGCENSUS_API EssClient& operator=(EssClient&&) = default;

    DBGCENSUS_API void connect();
    DBGCENSUS_API void disconnect();

    DBGCENSUS_API void setOnConnectCallback(std::function<void()> callback);
    DBGCENSUS_API void setOnDisconnectCallback(std::function<void()> callback);
    DBGCENSUS_API void setOnReadyCallback(std::function<void()> callback);
    DBGCENSUS_API void setOnMessageCallback(std::function<void(const std::string&)> callback);
    DBGCENSUS_API void setOnEventPayloadCallback(std::function<void(const std::string&)> callback);

    DBGCENSUS_API void send(const std::string& message);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace dbg_census::stream
