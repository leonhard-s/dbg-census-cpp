link_libraries(
    GTest::gtest_main
    DbgCensus::REST
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        -wd6326 # Potential comparison of a constant with another constant
    )
endif()

add_executable(UT_RetryStrategy unit/test_retry_strategy.cpp)
