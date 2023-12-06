#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <vector>
#include "dbg_census/stream/ess_client.h"


TEST(EssClient, ConnectToDefaultEndpoint) {
    dbg_census::stream::EssClient client;
    bool client_ready = false;
    client.setOnReadyCallback([&client_ready]() { client_ready = true; });

    using namespace std::chrono_literals;
    const std::chrono::milliseconds timeout{ 5s };
    const std::chrono::milliseconds check_interval{ 50ms };

    const auto start = std::chrono::steady_clock::now();
    client.connect();
    while(std::chrono::steady_clock::now() < start + timeout) {
        std::this_thread::sleep_for(check_interval);
        if(client_ready) {
            break;
        }
    }
    if(!client_ready) {
        FAIL() << "Client did not connect within " << timeout.count() << "ms";
    }
    client.disconnect();
}
