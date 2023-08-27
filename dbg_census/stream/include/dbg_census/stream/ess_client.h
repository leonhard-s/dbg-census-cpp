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

class DBGCENSUS_API EssClient {
public:
    explicit EssClient(
        const std::string& ess_service_id = detail::default_ess_service_id,
        const std::string& ess_environment = detail::default_ess_environment,
        const std::string& ess_endpoint = detail::default_ess_endpoint
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
    std::unique_ptr<Impl> m_impl;
};

} // namespace dbg_census::stream
