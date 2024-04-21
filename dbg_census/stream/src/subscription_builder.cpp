// Copyright 2023 Leonhard S.

#include "dbg_census/stream/subscription_builder.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace {

template<typename T>
std::string jsonStringArrayFromVector(const std::vector<T>& vector) {
    if (vector.empty()) {
        return "[]";
    }
    std::stringstream url;
    url << R"([")";
    std::copy(vector.begin(), vector.end() - 1, std::ostream_iterator<T>(url, R"(",")"));
    url << vector.back() << R"("])";
    return url.str();
}

} // Anonymous namespace

namespace dbg_census::stream {

SubscriptionBuilder::SubscriptionBuilder(const std::string& event_name)
    : m_event_names{ event_name }
{}

SubscriptionBuilder::SubscriptionBuilder(const std::vector<std::string>& event_names)
    : m_event_names{ event_names }
{}

SubscriptionBuilder& SubscriptionBuilder::setEventName(const std::string& event_name) {
    m_event_names = { event_name };
    return *this;
}

SubscriptionBuilder& SubscriptionBuilder::setEventNames(const std::vector<std::string>& event_names) {
    m_event_names = event_names;
    return *this;
}

SubscriptionBuilder& SubscriptionBuilder::setCharacters(const std::vector<detail::character_id_t>& character_ids, bool all) {
    if (all) {
        m_characters = { ALL_CHARACTERS };
    }
    else {
        m_characters.clear();
        std::transform(character_ids.begin(), character_ids.end(), std::back_inserter(m_characters), [](auto character_id) {
            return std::to_string(character_id);
            });
    }
    return *this;
}

SubscriptionBuilder& SubscriptionBuilder::setWorlds(const std::vector<detail::world_id_t>& world_ids, bool all) {
    if (all) {
        m_worlds = { ALL_WORLDS };
    }
    else {
        m_worlds.clear();
        std::transform(world_ids.begin(), world_ids.end(), std::back_inserter(m_worlds), [](auto world_id) {
            return std::to_string(world_id);
            });
    }
    return *this;
}

SubscriptionBuilder& SubscriptionBuilder::setLogicalAndCharactersWithWorlds(bool logical_and) {
    m_logical_and_characters_with_worlds = logical_and;
    return *this;
}

std::string SubscriptionBuilder::build() const {
    std::stringstream str;
    str << R"({"service":"event","action":"subscribe")";
    // Event names
    if (!m_event_names.empty()) {
        str << R"(,"eventNames":)" << jsonStringArrayFromVector(m_event_names);
    }
    // Characters
    if (!m_characters.empty()) {
        str << R"(,"characters":)" << jsonStringArrayFromVector(m_characters);
    }
    // Worlds
    if (!m_worlds.empty()) {
        str << R"(,"worlds":)" << jsonStringArrayFromVector(m_worlds);
    }
    // LogicalAnd
    if (m_logical_and_characters_with_worlds) {
        str << R"(,"logicalAndCharactersWithWorlds":true)";
    }
    str << "}";
    return str.str();
}

} // namespace dbg_census::stream
