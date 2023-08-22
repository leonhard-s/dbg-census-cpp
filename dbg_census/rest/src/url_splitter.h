// Copyright 2023 Leonhard S.

#pragma once

#include <string>
#include "dbg_census/rest/export.h"

namespace dbg_census::rest {

DBGCENSUS_API std::size_t findHostStartIndex(const std::string_view& url);

DBGCENSUS_API std::size_t findPathStartIndex(const std::string_view& url);

/**
 * @brief Return the host part of the URL.
 * @param url The URL to parse.
 * @param include_schema Whether to include the schema with the host.
 * @return The host part of the URL.
 */
DBGCENSUS_API std::string getHostFromUrl(const std::string& url, bool include_schema = false);

} // namespace dbg_census::rest
