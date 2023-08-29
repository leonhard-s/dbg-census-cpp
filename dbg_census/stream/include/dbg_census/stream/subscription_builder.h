// Copyright 2023 Leonhard S.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "dbg_census/stream/export.h"
#include "dbg_census/common/macros.h"

namespace dbg_census::stream {

namespace detail {

using world_id_t = int32_t;
using character_id_t = int64_t;

} // namespace detail

constexpr auto ALL_WORLDS = "all";
constexpr auto ALL_CHARACTERS = "all";

class DBGCENSUS_API SubscriptionBuilder {
public:
    SubscriptionBuilder() = default;
    explicit SubscriptionBuilder(const std::string& event_name);
    explicit SubscriptionBuilder(const std::vector<std::string>& event_names);

    virtual ~SubscriptionBuilder() = default;
    SubscriptionBuilder(const SubscriptionBuilder&) = delete;
    SubscriptionBuilder& operator=(const SubscriptionBuilder&) = delete;
    SubscriptionBuilder(SubscriptionBuilder&&) = default;
    SubscriptionBuilder& operator=(SubscriptionBuilder&&) = default;

    SubscriptionBuilder& setEventName(const std::string& event_name);
    SubscriptionBuilder& setEventNames(const std::vector<std::string>& event_names);
    SubscriptionBuilder& setCharacters(const std::vector<detail::character_id_t>& character_ids, bool all = false);
    SubscriptionBuilder& setWorlds(const std::vector<detail::world_id_t>& world_ids, bool all = false);
    SubscriptionBuilder& setLogicalAndCharactersWithWorlds(bool logical_and = true);

    std::string build() const;

private:
DISABLE_WARNING_4251
    std::vector<std::string> m_event_names;
    std::vector<std::string> m_worlds;
    std::vector<std::string> m_characters;
ENABLE_WARNING_4251
    bool m_logical_and_characters_with_worlds = false;
};

} // namespace dbg_census::stream
