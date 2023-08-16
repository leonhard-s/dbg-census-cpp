link_libraries(
    GTest::gtest_main
    DbgCensus::UrlGen
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        -wd6326 # Potential comparison of a constant with another constant
    )
endif()

add_executable(UT_UrlBuilder unit/test_url_builder.cpp)
