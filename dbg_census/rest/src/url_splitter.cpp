// Copyright 2023 Leonhard S.

#include "url_splitter.h"

#include <cstddef>
#include <string>
#include <string_view>

namespace dbg_census::rest {

std::size_t findHostStartIndex(const std::string_view& url) {
    const auto pos = url.find("://");
    if(pos == std::string::npos) {
        return 0;
    }
    return pos + 3;
}

std::size_t findPathStartIndex(const std::string_view& url) {
    const auto pos = url.find('/', findHostStartIndex(url));
    if(pos == std::string::npos) {
        return url.size();
    }
    return pos;
}

std::string getHostFromUrl(const std::string& url, bool include_schema) {
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
