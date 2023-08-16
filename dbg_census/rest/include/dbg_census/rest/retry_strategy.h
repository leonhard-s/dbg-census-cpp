// Copyright 2023 Leonhard S.

#pragma once

#include <chrono>

namespace dbg_census {

namespace rest {

/**
 * @brief A strategy for retrying failed HTTP requests.
*/
class RetryStrategy {
public:
    virtual ~RetryStrategy() = default;

    virtual bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const = 0;
    virtual std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const = 0;
};

// TODO (@leonhard-s): Add retry strategies

} // namespace rest

} // namespace dbg_census
