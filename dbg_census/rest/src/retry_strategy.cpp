// Copyright 2023 Leonhard S.

#include "dbg_census/rest/retry_strategy.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace dbg_census::rest {

auto NoRetry::shouldRetry(
    [[maybe_unused]] int status_code,
    [[maybe_unused]] std::size_t num_attempts,
    [[maybe_unused]] std::chrono::milliseconds total_delay
) const -> bool {
    return false;
}

auto NoRetry::getRetryDelay([[maybe_unused]] std::size_t num_attempts) const -> std::chrono::milliseconds {
    throw std::logic_error("NoRetry::getRetryDelay should never be called");
}

auto RetryOnce::shouldRetry(
    [[maybe_unused]] int status_code,
    std::size_t num_attempts,
    [[maybe_unused]] std::chrono::milliseconds total_delay
) const -> bool {
    return num_attempts == 0;
}

auto RetryOnce::getRetryDelay([[maybe_unused]] std::size_t num_attempts) const -> std::chrono::milliseconds {
    return std::chrono::milliseconds(0);
}

ExponentialBackoff::ExponentialBackoff(
    std::chrono::milliseconds initial_delay,
    double backoff_factor,
    std::chrono::milliseconds max_delay,
    std::size_t max_attempts,
    std::chrono::milliseconds max_total_delay
)
    : m_initial_delay(initial_delay)
    , m_backoff_factor(backoff_factor)
    , m_max_delay(max_delay)
    , m_max_attempts(max_attempts)
    , m_max_total_delay(max_total_delay)
{}

auto ExponentialBackoff::shouldRetry(
    [[maybe_unused]] int status_code,
    std::size_t num_attempts,
    std::chrono::milliseconds total_delay
) const -> bool {
    return num_attempts < m_max_attempts && (m_max_total_delay == std::chrono::milliseconds(0) || total_delay < m_max_total_delay);
}

auto ExponentialBackoff::getRetryDelay(std::size_t num_attempts) const -> std::chrono::milliseconds {
    auto delay = m_initial_delay * std::pow(m_backoff_factor, num_attempts - 1);
    return std::min(std::chrono::duration_cast<std::chrono::milliseconds>(delay), m_max_delay);
}

} // namespace dbg_census::rest
