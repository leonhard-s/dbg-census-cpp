// Copyright 2023 Leonhard S.

#pragma once

#include <string>

#include "dbg_census/stream/export.h"

namespace dbg_census::stream {

class DBGCENSUS_API EssClient {
public:
    EssClient(std::string service_id = "s:example");
    virtual ~EssClient() = default;
    
    // void Connect();
    // void Disconnect();
    
    // void Send(const char* data, size_t size);
};

} // namespace dbg_census::stream
