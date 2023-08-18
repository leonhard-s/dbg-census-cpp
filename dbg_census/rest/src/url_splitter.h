// Copyright 2023 Leonhard S.

#pragma once

#include <string>

namespace {

static inline std::size_t findHostStartIndex(const std::string_view& url) {
    const auto pos = url.find("://");
    if(pos == std::string::npos) {
        return 0;
    }
    return pos + 3;
}

static inline std::size_t findPathStartIndex(const std::string_view& url) {
    const auto pos = url.find('/', findHostStartIndex(url));
    if(pos == std::string::npos) {
        return url.size();
    }
    return pos;
}

} // anonymous namespace

namespace dbg_census::rest
{

/**
 * @brief Return the host part of the URL.
 * @param url The URL to parse.
 * @param include_schema Whether to include the schema with the host.
 * @return The host part of the URL.
 */
static std::string getHostFromUrl(const std::string& url, bool include_schema = false) {
    const auto start = findHostStartIndex(url);
    auto end = url.find_first_of('/', start);
    if(end == std::string::npos) {
        end = url.size();
    }
    if(include_schema) {
        return url.substr(0, end);
    }
    return url.substr(start, end - start);
}

} // namespace dbg_census::rest
