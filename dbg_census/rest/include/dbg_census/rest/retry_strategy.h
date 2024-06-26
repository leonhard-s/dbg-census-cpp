// Copyright 2023 Leonhard S.

#pragma once

#include <chrono>
#include "dbg_census/rest/export.h"
#include "dbg_census/rest/http_status.h"
#include "dbg_census/common/macros.h"

namespace dbg_census::rest {

/**
 * @brief A strategy for retrying failed HTTP requests.
*/
class DBGCENSUS_API RetryStrategy {
public:
    RetryStrategy() = default;
    virtual ~RetryStrategy() = default;
    RetryStrategy(const RetryStrategy&) = default;
    RetryStrategy(RetryStrategy&&) = default;
    RetryStrategy& operator=(const RetryStrategy&) = default;
    RetryStrategy& operator=(RetryStrategy&&) = default;

    virtual bool shouldRetry(HttpStatusCode status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const = 0;
    virtual std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const = 0;
};

class DBGCENSUS_API NoRetry : public RetryStrategy {
public:
    NoRetry() = default;
    bool shouldRetry(HttpStatusCode status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;
};

class DBGCENSUS_API RetryOnce : public RetryStrategy {
public:
    RetryOnce() = default;
    bool shouldRetry(HttpStatusCode status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;
};

class DBGCENSUS_API ExponentialBackoff : public RetryStrategy {
public:
    ExponentialBackoff() = delete;
    ExponentialBackoff(std::chrono::milliseconds initial_delay, double backoff_factor, std::chrono::milliseconds max_delay, std::size_t max_attempts, std::chrono::milliseconds max_total_delay = std::chrono::milliseconds(0));

    bool shouldRetry(HttpStatusCode status_code, std::size_t num_attempts, std::chrono::milliseconds total_delay) const override;
    std::chrono::milliseconds getRetryDelay(std::size_t num_attempts) const override;

private:
    DISABLE_WARNING_4251;
    std::chrono::milliseconds m_initial_delay;
    double m_backoff_factor;
    std::chrono::milliseconds m_max_delay;
    std::size_t m_max_attempts;
    std::chrono::milliseconds m_max_total_delay;
    ENABLE_WARNING_4251;
};

} // namespace dbg_census::rest
