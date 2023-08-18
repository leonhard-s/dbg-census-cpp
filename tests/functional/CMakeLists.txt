link_libraries(GTest::gtest_main)

add_executable(FT_RestApiClient test_rest_api_client.cpp)
target_link_libraries(FT_RestApiClient DbgCensus::REST DbgCensus::UrlGen)
add_test(NAME FT_RestApiClient COMMAND FT_RestApiClient)
