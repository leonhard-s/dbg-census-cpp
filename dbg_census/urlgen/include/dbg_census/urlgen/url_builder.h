// Copyright 2023 Leonhard S.

#pragma once

#include <string>
#include <utility>
#include <vector>

namespace dbg_census::urlgen {

namespace detail {

static const std::string default_service_id = "s:example";
static const std::string default_base_url = "https://census.daybreakgames.com";
static const std::string default_query_verb = "get";

} // namespace detail

/**
 * @brief A builder class for generating Census REST API URLs.
 *
 * @note This class is not thread-safe.
*/
class UrlBuilder {
public:
    UrlBuilder();
    UrlBuilder(
        const std::string& ns,
        const std::string& collection = "",
        const std::string& service_id = detail::default_service_id
    );

    virtual ~UrlBuilder() = default;
    UrlBuilder(const UrlBuilder& other) = default;
    UrlBuilder(UrlBuilder&& other) = default;
    UrlBuilder& operator=(const UrlBuilder& other) = default;
    UrlBuilder& operator=(UrlBuilder&& other) = default;

    /**
     * @brief Sets the base URL of the Census REST API endpoint.
     * @param base_url Base URL including schema and without trailing slash,
     * e.g. https://census.daybreakgames.com
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& setBaseUrl(const std::string& base_url);

    /**
     * @brief Sets the service ID used to identify this service.
     * @param service_id New service ID. If the "s:" prefix is omitted, it will
     * be added automatically.
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& setServiceId(const std::string& service_id);

    /**
     * @brief Sets the query verb used for the query.
     * @param verb The query verb, such as "get" or "count".
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& setQueryVerb(const std::string& verb);

    /**
     * @brief Sets the namespace (aka. game) of the query.
     * @param ns The API namespace to access, such as "ps2".
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& setNamespace(const std::string& ns);

    /**
     * @brief Sets the collection (aka. table) of the query.
     * @param endpoint The API endpoint to access, such as "character". Set to
     * an empty string to omit the collection from the URL path.
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& setCollection(const std::string& endpoint);

    /**
     * @brief Adds a query term to the URL.
     * @param field The field to query. Supports nested fields such as
     * "name.first".
     * @param value The value to query for. Prefix the value with the search
     * modifier to use, such as "!" for negation or "*" for a string wildcard.
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& addQueryTerm(const std::string& field, const std::string& value);

    /**
     * @brief Adds multiple query terms to the URL.
     * @param queryTerms A vector of field-value pairs to add to the query.
     * See `addQueryTerm()` for more information on the expected format.
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& addQueryTerms(const std::vector<std::pair<std::string, std::string>>& queryTerms);

    /**
     * @brief Adds a query command to the URL.
     * @param command The command to add to the query, such as "limit=10".
     * If the `c:` prefix is omitted, it will be added automatically.
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& addQueryCommand(const std::string& command);

    /**
     * @brief Adds multiple query commands to the URL.
     * @param queryCommands A vector of query commands to add to the query.
     * See `addQueryCommand() for more information on the expected format.
     * @return A reference to the current object.
     * @note This method supports chaining.
    */
    UrlBuilder& addQueryCommands(const std::vector<std::string>& queryCommands);

    /**
     * @brief Generates the URL from the current state of the builder.
     * @return The generated URL.
    */
    std::string build() const;

private:
    std::string m_base_url;
    std::string m_service_id;
    std::string m_query_verb;
    std::string m_namespace;
    std::string m_collection;
    std::vector<std::pair<std::string, std::string>> m_query_terms;
    std::vector<std::string> m_query_commands;
};

UrlBuilder::UrlBuilder()
    : m_base_url{ detail::default_base_url }
    , m_service_id{ detail::default_service_id }
    , m_query_verb{ detail::default_query_verb }
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
    : m_base_url{ detail::default_base_url }
    , m_service_id{ service_id }
    , m_query_verb{ detail::default_query_verb }
    , m_namespace{ ns }
    , m_collection{ collection }
    , m_query_terms{}
    , m_query_commands{}
{
    if(!m_service_id.starts_with("s:"))
        m_service_id = "s:" + m_service_id;
}

UrlBuilder& UrlBuilder::setBaseUrl(const std::string& base_url) {
    m_base_url = base_url;
    return *this;
}

UrlBuilder& UrlBuilder::setServiceId(const std::string& service_id) {
    if(service_id.starts_with("s:"))
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
    if(m_collection.size() > 0)
        url << "/" << m_collection;
    // Query terms (optional)
    url << "?";
    if(m_query_terms.size() > 0) {
        for(auto& term : m_query_terms) {
            url << term.first << "=" << term.second << "&";
        }
    }
    // Query commands (optional)
    if(m_query_commands.size() > 0) {
        for(auto& command : m_query_commands) {
            if(!command.starts_with("c:"))
                url << "c:";
            url << command << "&";
        }
    }
    // Convert to string, skipping the last character if it's a '&' or '?'
    auto url_str = url.str();
    if(url_str.back() == '&' || url_str.back() == '?')
        return url_str.substr(0, url_str.size() - 1);
    return url_str;
}

} // namespace dbg_census::urlgen
