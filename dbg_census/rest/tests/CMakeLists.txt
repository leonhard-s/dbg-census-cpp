link_libraries(
    GTest::gtest_main
    DbgCensus::REST
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        -wd6326 # Potential comparison of a constant with another constant
    )
endif()

add_executable(UT_RetryStrategy unit/test_retry_strategy.cpp)
add_test(NAME UT_RetryStrategy COMMAND UT_RetryStrategy)
add_executable(UT_RestApiClient unit/test_rest_api_client.cpp)
add_test(NAME UT_RestApiClient COMMAND UT_RestApiClient)

add_executable(UT_HttpClientAdapter unit/test_http_client_adapter.cpp
    # TODO (leonhard-s): Reuse OBJ files rather than recompiling the sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/http_client_adapter.cpp
)
target_link_libraries(UT_HttpClientAdapter PRIVATE ixwebsocket::ixwebsocket)
add_test(NAME UT_HttpClientAdapter COMMAND UT_HttpClientAdapter)
