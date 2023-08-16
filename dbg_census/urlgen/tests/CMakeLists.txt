link_libraries(
    GTest::gtest_main
    DbgCensus::UrlGen
)

add_executable(UT_UrlBuilder unit/test_url_builder.cpp)
