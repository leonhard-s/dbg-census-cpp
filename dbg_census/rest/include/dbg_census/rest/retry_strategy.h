// Copyright 2023 Leonhard S.

#pragma once

#include <chrono>
#include "dbg_census/rest/export.h"

namespace dbg_census::rest {

/**
 * @brief A strategy for retrying failed HTTP requests.
*/
class DBGCENSUS_API RetryStrategy {
public:
    virtual ~RetryStrategy() = default;

    virtual bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const = 0;
    virtual std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const = 0;
};

class DBGCENSUS_API NoRetry : public RetryStrategy {
public:
    bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;
};

class DBGCENSUS_API RetryOnce : public RetryStrategy {
public:
    bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;
};

class DBGCENSUS_API ExponentialBackoff : public RetryStrategy {
public:
    ExponentialBackoff(std::chrono::milliseconds initial_delay, double backoff_factor, std::chrono::milliseconds max_delay, std::size_t max_attempts, std::chrono::milliseconds max_total_delay = std::chrono::milliseconds(0));

    bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;

private:
    std::chrono::milliseconds m_initial_delay;
    double m_backoff_factor;
    std::chrono::milliseconds m_max_delay;
    std::size_t m_max_attempts;
    std::chrono::milliseconds m_max_total_delay;
};

} // namespace dbg_census::rest
