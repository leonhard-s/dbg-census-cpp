#include <gtest/gtest.h>

#include "dbg_census/rest/retry_strategy.h"

TEST(RetryStrategy, NoRetryNeverRetries) {
    const dbg_census::rest::NoRetry strategy;
    EXPECT_FALSE(strategy.shouldRetry(0, 0, std::chrono::milliseconds(0)));
    // NoRetry::getRetryDelay not tested as it is never called
}

TEST(RetryStrategy, RetryOnceRetriesOnce) {
    const dbg_census::rest::RetryOnce strategy;
    EXPECT_TRUE(strategy.shouldRetry(0, 0, std::chrono::milliseconds(0)));
    EXPECT_FALSE(strategy.shouldRetry(0, 1, std::chrono::milliseconds(0)));
}

TEST(RetryStrategy, RetryOnceRetriesWithoutDelay) {
    const dbg_census::rest::RetryOnce strategy;
    EXPECT_EQ(std::chrono::milliseconds(0), strategy.getRetryDelay(0));
    EXPECT_EQ(std::chrono::milliseconds(0), strategy.getRetryDelay(1));
}

TEST(RetryStrategy, ExponentialBackoffConstant) {
    const std::size_t max_attempts = 10;
    const dbg_census::rest::ExponentialBackoff strategy(
        std::chrono::milliseconds(100),  // Initial delay
        1.0,                             // Base
        std::chrono::milliseconds(1000), // Max delay
        max_attempts                               // Max attempts
    );
    for(std::size_t i = 0; i < max_attempts; ++i) {
        EXPECT_TRUE(strategy.shouldRetry(0, i, std::chrono::milliseconds(100 * i)));
        EXPECT_EQ(std::chrono::milliseconds(100), strategy.getRetryDelay(i));
    }
    EXPECT_FALSE(strategy.shouldRetry(0, 10, std::chrono::milliseconds(1000)));
}

TEST(RetryStrategy, ExponentialBackoffBase2) {
    const dbg_census::rest::ExponentialBackoff strategy(
        std::chrono::milliseconds(100),  // Initial delay
        2.0,                             // Base
        std::chrono::milliseconds(1000), // Max delay
        4                                // Max attempts
    );
    EXPECT_TRUE(strategy.shouldRetry(0, 0, std::chrono::milliseconds(0)));
    EXPECT_EQ(std::chrono::milliseconds(100), strategy.getRetryDelay(1));
    EXPECT_TRUE(strategy.shouldRetry(0, 1, std::chrono::milliseconds(100)));
    EXPECT_EQ(std::chrono::milliseconds(200), strategy.getRetryDelay(2));
    EXPECT_TRUE(strategy.shouldRetry(0, 2, std::chrono::milliseconds(300)));
    EXPECT_EQ(std::chrono::milliseconds(400), strategy.getRetryDelay(3));
    EXPECT_TRUE(strategy.shouldRetry(0, 2, std::chrono::milliseconds(700)));
    EXPECT_EQ(std::chrono::milliseconds(800), strategy.getRetryDelay(4));
}

TEST(RetryStrategy, ExponentialBackoffBase10) {
    const dbg_census::rest::ExponentialBackoff strategy(
        std::chrono::milliseconds(10),   // Initial delay
        10.0,                            // Base
        std::chrono::milliseconds(1000), // Max delay
        4                                // Max attempts
    );
    EXPECT_TRUE(strategy.shouldRetry(0, 0, std::chrono::milliseconds(0)));
    EXPECT_EQ(std::chrono::milliseconds(10), strategy.getRetryDelay(1));
    EXPECT_TRUE(strategy.shouldRetry(0, 1, std::chrono::milliseconds(10)));
    EXPECT_EQ(std::chrono::milliseconds(100), strategy.getRetryDelay(2));
    EXPECT_TRUE(strategy.shouldRetry(0, 2, std::chrono::milliseconds(110)));
    EXPECT_EQ(std::chrono::milliseconds(1000), strategy.getRetryDelay(3));
    EXPECT_TRUE(strategy.shouldRetry(0, 2, std::chrono::milliseconds(1110)));
    EXPECT_EQ(std::chrono::milliseconds(1000), strategy.getRetryDelay(4));
}

TEST(RetryStrategy, ExponentialBackoffMaxAttempts) {
    const dbg_census::rest::ExponentialBackoff strategy(
        std::chrono::milliseconds(10),   // Initial delay
        1.0,                             // Base
        std::chrono::milliseconds(1000), // Max delay
        4                                // Max attempts
    );
    EXPECT_TRUE(strategy.shouldRetry(0, 0, std::chrono::milliseconds(0)));
    EXPECT_TRUE(strategy.shouldRetry(0, 1, std::chrono::milliseconds(10)));
    EXPECT_TRUE(strategy.shouldRetry(0, 2, std::chrono::milliseconds(20)));
    EXPECT_TRUE(strategy.shouldRetry(0, 3, std::chrono::milliseconds(30)));
    EXPECT_FALSE(strategy.shouldRetry(0, 4, std::chrono::milliseconds(40)));
}

TEST(RetryStrategy, ExponentialBackoffMaxTotalDelay) {
    const dbg_census::rest::ExponentialBackoff strategy(
        std::chrono::milliseconds(10),// Initial delay
        1.0,                          // Base
        std::chrono::milliseconds(10),// Max delay
        10,                           // Max attempts
        std::chrono::milliseconds(50) // Max total delay
    );
    const auto expected_attempts = 5;
    for(std::size_t i = 0; i < expected_attempts; ++i) {
        EXPECT_TRUE(strategy.shouldRetry(0, i, std::chrono::milliseconds(10 * i)));
    }
    EXPECT_FALSE(strategy.shouldRetry(0, 6, std::chrono::milliseconds(50)));
}
