// Copyright 2023 Leonhard S.

#pragma once

#include <string>
#include <utility>
#include <vector>

namespace dbg_census {

namespace urlgen {

static const std::string default_service_id = "s:example";

class UrlBuilder {
public:
    UrlBuilder();
    UrlBuilder(
        const std::string& ns,
        const std::string& collection = "",
        const std::string& service_id = default_service_id
    );

    virtual ~UrlBuilder() = default;
    UrlBuilder(const UrlBuilder& other);
    UrlBuilder(UrlBuilder&& other);
    UrlBuilder& operator=(const UrlBuilder& other);
    UrlBuilder& operator=(UrlBuilder&& other);

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

} // namespace urlgen

} // namespace dbg_census
