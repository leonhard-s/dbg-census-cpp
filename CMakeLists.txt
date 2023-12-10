cmake_minimum_required(VERSION 3.20)
project(DbgCensus VERSION 0.2.0 LANGUAGES CXX)

# Project-wide configuration
option(BUILD_TESTING "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>)

# Dependencies
add_subdirectory(3rdparty EXCLUDE_FROM_ALL)

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

# Build targets
add_subdirectory(dbg_census/common)
add_subdirectory(dbg_census/urlgen)
add_subdirectory(dbg_census/rest)
add_subdirectory(dbg_census/stream)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests/functional)

    # TODO (@leonhard-s): Integration tests
endif()

# Installation
include(GNUInstallDirs)
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "cmake/${PROJECT_NAME}Config.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    # TODO (leonhard-s): Use "SameMajorVersion" once we have a stable API
    COMPATIBILITY "SameMinorVersion"
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES "LICENSE" "README.md" DESTINATION "share")

string(APPEND CMAKE_INSTALL_BINDIR /$<CONFIG>)
string(APPEND CMAKE_INSTALL_LIBDIR /$<CONFIG>)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(RUNTIME_DEPENDENCY_DIR ${CMAKE_INSTALL_BINDIR})
else()
    set(RUNTIME_DEPENDENCY_DIR ${CMAKE_INSTALL_LIBDIR})
endif()

install(RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}Deps
  PRE_EXCLUDE_REGEXES
    [[api-ms-win-.*]] [[ext-ms-.*]] [[kernel32\.dll]] [[clang_rt.asan.*]]
  POST_EXCLUDE_REGEXES
    [[.*/system32/.*\.dll]] [[bcrypt\.dll]] [[bcryptprimitives\.dll]]
    [[advapi32\.dll]] [[crypt32\.dll]] [[msvcp140d?\.dll]] [[shlwapi\.dll]]
    [[ucrtbased?\.dll]] [[user32\.dll]] [[vcruntime140(_1)?d?\.dll]]
    [[ws2_32\.dll]] [[wsock32\.dll]]
  DESTINATION ${RUNTIME_DEPENDENCY_DIR}
)
