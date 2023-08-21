cmake_minimum_required(VERSION 3.20)
project(DbgCensus VERSION 0.1.0 LANGUAGES CXX)

# Project-wide configuration
option(BUILD_TESTING "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Compiler options
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC))
    add_compile_options(
        /Wall
        /permissive-

        # Disable specific warnings
        /wd5045 # 'Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified'
    )
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Dependencies
find_package(GTest)
find_package(httplib REQUIRED)
find_package(OpenSSL)

# Build targets
add_subdirectory(dbg_census/urlgen)
add_subdirectory(dbg_census/rest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests/functional)

    # TODO (@leonhard-s): Integration tests
endif()

# Installation
install(EXPORT ${PROJECT_NAME}Config
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)
install(FILES "LICENSE" "README.md" DESTINATION "share")

# TODO (@leonhard-s): Support packaging
