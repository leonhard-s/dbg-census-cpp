// Copyright 2023 Leonhard S.

#pragma once

#include <chrono>
#include "dbg_census/rest/export.h"

namespace dbg_census {

namespace rest {

/**
 * @brief A strategy for retrying failed HTTP requests.
*/
class RetryStrategy {
public:
    virtual ~RetryStrategy() = default;

    DBGCENSUS_API virtual bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const = 0;
    DBGCENSUS_API virtual std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const = 0;
};

class NoRetry : public RetryStrategy {
public:
    DBGCENSUS_API bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    DBGCENSUS_API std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;
};

class RetryOnce : public RetryStrategy {
public:
    DBGCENSUS_API bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    DBGCENSUS_API std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;
};

class ExponentialBackoff : public RetryStrategy {
public:
    DBGCENSUS_API ExponentialBackoff(std::chrono::milliseconds initial_delay, double backoff_factor, std::chrono::milliseconds max_delay, std::size_t max_attempts, std::chrono::milliseconds max_total_delay = std::chrono::milliseconds(0));

    DBGCENSUS_API bool shouldRetry(int status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    DBGCENSUS_API std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;

private:
    std::chrono::milliseconds m_initial_delay;
    double m_backoff_factor;
    std::chrono::milliseconds m_max_delay;
    std::size_t m_max_attempts;
    std::chrono::milliseconds m_max_total_delay;
};

} // namespace rest

} // namespace dbg_census
