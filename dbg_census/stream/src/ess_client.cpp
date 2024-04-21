// Copyright 2023 Leonhard S.

#include "dbg_census/stream/ess_client.h"

#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketMessage.h>
#include <ixwebsocket/IXWebSocketMessageType.h>

namespace dbg_census::stream {

class EssClient::Impl {
public:
    Impl(std::string ess_service_id,
        std::string ess_environment,
        std::string ess_endpoint)
        : m_ess_service_id{ std::move(ess_service_id) }
        , m_ess_environment{ std::move(ess_environment) }
        , m_ess_endpoint{ std::move(ess_endpoint) }
        , m_ws_client{ nullptr }
    {}

    ~Impl() = default;
    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;
    Impl(Impl&&) = default;
    Impl& operator=(Impl&&) = default;

    void connect() {
        ix::initNetSystem();

        m_ws_client = std::make_unique<ix::WebSocket>();
        m_ws_client->setUrl(getEssUrl());
        m_ws_client->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            if(msg->type == ix::WebSocketMessageType::Message) {
                auto const& msg_str = msg->str;
                if(m_on_message_callback) {
                    m_on_message_callback(msg_str);
                }

                // Check for "serviceMessage" payload
                if(msg_str.find(R"("type":"serviceMessage")") != std::string::npos) {
                    if(m_on_event_payload_callback) {
                        m_on_event_payload_callback(msg_str);
                    }
                }

                // Check for "ready" payload
                if(msg_str.find(R"("send this for help")") != std::string::npos) {
                    if(m_on_ready) {
                        m_on_ready();
                    }
                }
            }
            else if(msg->type == ix::WebSocketMessageType::Open) {
                if(m_on_connect_callback) {
                    m_on_connect_callback();
                }
            }
            else if(msg->type == ix::WebSocketMessageType::Error) {
                // TODO: Error handling (payload-level)
            }
            });

        try {
            m_ws_client->start();
        }
        catch(const std::exception& e) {
            // TODO: Error handling (connection-level)
            throw e;
        }
    }

    void disconnect() {
        // m_ws_client->stop();
        m_ws_client = nullptr;
        ix::uninitNetSystem();
    }

    void setOnConnectCallback(std::function<void()> callback) {
        m_on_connect_callback = std::move(callback);
    }

    void setOnDisconnectCallback(std::function<void()> callback) {
        m_on_disconnect_callback = std::move(callback);
    }

    void setOnReadyCallback(std::function<void()> callback) {
        m_on_ready = std::move(callback);
    }

    void setOnMessageCallback(std::function<void(const std::string&)> callback) {
        m_on_message_callback = std::move(callback);
    }

    void setOnEventPayloadCallback(std::function<void(const std::string&)> callback) {
        m_on_event_payload_callback = std::move(callback);
    }

    void send(const std::string& message) {
        m_ws_client->send(message);
    }

private:
    std::string m_ess_service_id;
    std::string m_ess_environment;
    std::string m_ess_endpoint;
    std::unique_ptr<ix::WebSocket> m_ws_client;
    std::function<void()> m_on_connect_callback;
    std::function<void()> m_on_disconnect_callback;
    std::function<void()> m_on_ready;
    std::function<void(const std::string&)> m_on_message_callback;
    std::function<void(const std::string&)> m_on_event_payload_callback;

    [[nodiscard]] std::string getEssUrl() const {
        std::stringstream url;
        url << m_ess_endpoint << "?environment=" << m_ess_environment << "&service-id=" << m_ess_service_id;
        return url.str();
    }
};

EssClient::EssClient(const std::string& ess_service_id,
    const std::string& ess_environment,
    const std::string& ess_endpoint)
    : m_impl{ std::make_unique<Impl>(ess_service_id, ess_environment, ess_endpoint) }
{}

EssClient::~EssClient() = default;

void EssClient::connect() {
    m_impl->connect();
}

void EssClient::disconnect() {
    m_impl->disconnect();
}

void EssClient::setOnConnectCallback(std::function<void()> callback) {
    m_impl->setOnConnectCallback(std::move(callback));
}

void EssClient::setOnDisconnectCallback(std::function<void()> callback) {
    m_impl->setOnDisconnectCallback(std::move(callback));
}

void EssClient::setOnReadyCallback(std::function<void()> callback) {
    m_impl->setOnReadyCallback(std::move(callback));
}

void EssClient::setOnMessageCallback(std::function<void(const std::string&)> callback) {
    m_impl->setOnMessageCallback(std::move(callback));
}

void EssClient::setOnEventPayloadCallback(std::function<void(const std::string&)> callback) {
    m_impl->setOnEventPayloadCallback(std::move(callback));
}

void EssClient::send(const std::string& message) {
    m_impl->send(message);
}

} // namespace dbg_census::stream
