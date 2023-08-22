link_libraries(GTest::gtest_main)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        -wd6326 # Potential comparison of a constant with another constant
    )
endif()

add_executable(FT_RestApiClient test_rest_api_client.cpp)
target_link_libraries(FT_RestApiClient DbgCensus::REST DbgCensus::UrlGen)
add_test(NAME FT_RestApiClient COMMAND FT_RestApiClient)

add_executable(FT_UrlGen test_urlgen.cpp)
target_link_libraries(FT_UrlGen DbgCensus::UrlGen)
add_test(NAME FT_UrlGen COMMAND FT_UrlGen)
