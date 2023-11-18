// Copyright 2023 Leonhard S.

#include "dbg_census/stream/subscription_builder.h"
#include <algorithm>
#include <sstream>

namespace {

template<typename T>
auto jsonStringArrayFromVector(const std::vector<T>& vector) -> std::string {
    if (vector.empty()) {
        return "[]";
    }
    std::stringstream ss;
    ss << R"([")";
    std::copy(vector.begin(), vector.end() - 1, std::ostream_iterator<T>(ss, R"(",")"));
    ss << vector.back() << R"("])";
    return ss.str();
}

} // Anonymous namespace

namespace dbg_census::stream {

SubscriptionBuilder::SubscriptionBuilder(const std::string& event_name)
    : m_event_names{ event_name }
{}

SubscriptionBuilder::SubscriptionBuilder(const std::vector<std::string>& event_names)
    : m_event_names{ event_names }
{}

auto SubscriptionBuilder::setEventName(const std::string& event_name) -> SubscriptionBuilder& {
    m_event_names = { event_name };
    return *this;
}

auto SubscriptionBuilder::setEventNames(const std::vector<std::string>& event_names) -> SubscriptionBuilder& {
    m_event_names = event_names;
    return *this;
}

auto SubscriptionBuilder::setCharacters(const std::vector<detail::character_id_t>& character_ids, bool all) -> SubscriptionBuilder& {
    if (all) {
        m_characters = { ALL_CHARACTERS };
    }
    else {
        m_characters.clear();
        std::transform(character_ids.begin(), character_ids.end(), std::back_inserter(m_characters), [](auto id) {
            return std::to_string(id);
            });
    }
    return *this;
}

auto SubscriptionBuilder::setWorlds(const std::vector<detail::world_id_t>& world_ids, bool all) -> SubscriptionBuilder& {
    if (all) {
        m_worlds = { ALL_WORLDS };
    }
    else {
        m_worlds.clear();
        std::transform(world_ids.begin(), world_ids.end(), std::back_inserter(m_worlds), [](auto id) {
            return std::to_string(id);
            });
    }
    return *this;
}

auto SubscriptionBuilder::setLogicalAndCharactersWithWorlds(bool logical_and) -> SubscriptionBuilder& {
    m_logical_and_characters_with_worlds = logical_and;
    return *this;
}

auto SubscriptionBuilder::build() const -> std::string {
    std::stringstream ss;
    ss << R"({"service":"event","action":"subscribe")";
    // Event names
    if (!m_event_names.empty()) {
        ss << R"(,"eventNames":)" << jsonStringArrayFromVector(m_event_names);
    }
    // Characters
    if (!m_characters.empty()) {
        ss << R"(,"characters":)" << jsonStringArrayFromVector(m_characters);
    }
    // Worlds
    if (!m_worlds.empty()) {
        ss << R"(,"worlds":)" << jsonStringArrayFromVector(m_worlds);
    }
    // LogicalAnd
    if (m_logical_and_characters_with_worlds) {
        ss << R"(,"logicalAndCharactersWithWorlds":true)";
    }
    ss << "}";
    return ss.str();
}

} // namespace dbg_census::stream
