// Copyright 2023 Leonhard S.

#pragma once

#include "dbg_census/rest/export.h"

namespace dbg_census::rest {

enum class HttpStatusCode {
    // 1xx Informational
    // 2xx Success
    OK = 200,
    // 3xx Redirection
    // 4xx Client Error
    // 5xx Server Error
    // 6xx Custom Error
    RETRY = 600,
};

} // namespace dbg_census::rest
