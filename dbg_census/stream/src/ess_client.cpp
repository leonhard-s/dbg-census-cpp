// Copyright 2023 Leonhard S.

#include "dbg_census/stream/ess_client.h"
#include <sstream>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

namespace dbg_census::stream {

class EssClient::Impl {
public:
    Impl(const std::string& ess_service_id,
        const std::string& ess_environment,
        const std::string& ess_endpoint)
        : m_ess_service_id{ ess_service_id }
        , m_ess_environment{ ess_environment }
        , m_ess_endpoint{ ess_endpoint }
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
                if(m_on_message_callback)
                    m_on_message_callback(msg_str);

                // Check for "serviceMessage" payload
                if(msg_str.find("\"type\":\"serviceMessage\"") != std::string::npos) {
                    if(m_on_event_payload_callback)
                        m_on_event_payload_callback(msg_str);
                }

                // Check for "ready" payload
                if(msg_str.find("\"send this for help\"") != std::string::npos) {
                    if(m_on_ready)
                        m_on_ready();
                }
            }
            else if(msg->type == ix::WebSocketMessageType::Open) {
                if(m_on_connect_callback)
                    m_on_connect_callback();
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
        m_on_connect_callback = callback;
    }

    void setOnDisconnectCallback(std::function<void()> callback) {
        m_on_disconnect_callback = callback;
    }

    void setOnReadyCallback(std::function<void()> callback) {
        m_on_ready = callback;
    }

    void setOnMessageCallback(std::function<void(const std::string&)> callback) {
        m_on_message_callback = callback;
    }

    void setOnEventPayloadCallback(std::function<void(const std::string&)> callback) {
        m_on_event_payload_callback = callback;
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

    std::string getEssUrl() const {
        std::stringstream ss;
        ss << m_ess_endpoint << "?environment=" << m_ess_environment << "&service-id=" << m_ess_service_id;
        return ss.str();
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
    m_impl->setOnConnectCallback(callback);
}

void EssClient::setOnDisconnectCallback(std::function<void()> callback) {
    m_impl->setOnDisconnectCallback(callback);
}

void EssClient::setOnReadyCallback(std::function<void()> callback) {
    m_impl->setOnReadyCallback(callback);
}

void EssClient::setOnMessageCallback(std::function<void(const std::string&)> callback) {
    m_impl->setOnMessageCallback(callback);
}

void EssClient::setOnEventPayloadCallback(std::function<void(const std::string&)> callback) {
    m_impl->setOnEventPayloadCallback(callback);
}

void EssClient::send(const std::string& message) {
    m_impl->send(message);
}

} // namespace dbg_census::stream
