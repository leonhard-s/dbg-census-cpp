// Copyright 2023 Leonhard S.

#include "dbg_census/urlgen/url_builder.h"

#include <sstream>

namespace dbg_census {

namespace urlgen {

static const std::string default_base_url = "https://census.daybreakgames.com";
static const std::string default_query_verb = "get";

UrlBuilder::UrlBuilder()
    : m_base_url{ default_base_url }
    , m_service_id{ detail::default_service_id }
    , m_query_verb{ default_query_verb }
    , m_namespace{ "" }
    , m_collection{ "" }
    , m_query_terms{}
    , m_query_commands{}
{}

UrlBuilder::UrlBuilder(
    const std::string& ns,
    const std::string& collection,
    const std::string& service_id
)
    : m_base_url{ default_base_url }
    , m_service_id{ service_id }
    , m_query_verb{ default_query_verb }
    , m_namespace{ ns }
    , m_collection{ collection }
    , m_query_terms{}
    , m_query_commands{}
{
    if (!m_service_id.starts_with("s:"))
        m_service_id = "s:" + m_service_id;
}

UrlBuilder::UrlBuilder(const UrlBuilder& other) = default;

UrlBuilder::UrlBuilder(UrlBuilder&& other) = default;

UrlBuilder& UrlBuilder::operator=(const UrlBuilder& other) = default;

UrlBuilder& UrlBuilder::operator=(UrlBuilder&& other) = default;

UrlBuilder& UrlBuilder::setBaseUrl(const std::string& base_url) {
    m_base_url = base_url;
    return *this;
}

UrlBuilder& UrlBuilder::setServiceId(const std::string& service_id) {
    if (service_id.starts_with("s:"))
        m_service_id = service_id;
    else
        m_service_id = "s:" + service_id;
    return *this;
}

UrlBuilder& UrlBuilder::setQueryVerb(const std::string& verb) {
    m_query_verb = verb;
    return *this;
}

UrlBuilder& UrlBuilder::setNamespace(const std::string& ns) {
    m_namespace = ns;
    return *this;
}

UrlBuilder& UrlBuilder::setCollection(const std::string& collection) {
    m_collection = collection;
    return *this;
}

UrlBuilder& UrlBuilder::addQueryTerm(const std::string& key, const std::string& value) {
    m_query_terms.emplace_back(std::make_pair(key, value));
    return *this;
}

UrlBuilder& UrlBuilder::addQueryTerms(
    const std::vector<std::pair<std::string, std::string>>& queryTerms
) {
    m_query_terms.insert(m_query_terms.end(), queryTerms.begin(), queryTerms.end());
    return *this;
}

UrlBuilder& UrlBuilder::addQueryCommand(const std::string& command) {
    m_query_commands.emplace_back(command);
    return *this;
}

UrlBuilder& UrlBuilder::addQueryCommands(const std::vector<std::string>& commands) {
    m_query_commands.insert(m_query_commands.end(), commands.begin(), commands.end());
    return *this;
}

std::string UrlBuilder::build() const {
    std::stringstream url;
    // Required components
    url << m_base_url << "/" << m_service_id << "/" << m_query_verb << "/" << m_namespace;
    // Collection (optional)
    if (m_collection.size() > 0)
        url << "/" << m_collection;
    // Query terms (optional)
    url << "?";
    if (m_query_terms.size() > 0) {
        for (auto& term : m_query_terms) {
            url << term.first << "=" << term.second << "&";
        }
    }
    // Query commands (optional)
    if (m_query_commands.size() > 0) {
        for (auto& command : m_query_commands) {
            if (!command.starts_with("c:"))
                url << "c:";
            url << command << "&";
        }
    }
    // Convert to string, skipping the last character if it's a '&' or '?'
    auto url_str = url.str();
    if (url_str.back() == '&' || url_str.back() == '?')
        return url_str.substr(0, url_str.size() - 1);
    return url_str;
}

} // namespace urlgen

} // namespace dbg_census
