link_libraries(
    GTest::gtest_main
    DbgCensus::Stream
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        -wd6326 # Potential comparison of a constant with another constant
    )
endif()

add_executable(UT_EssClient unit/test_ess_client.cpp)
add_test(NAME UT_EssClient COMMAND UT_EssClient)
