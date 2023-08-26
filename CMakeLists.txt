cmake_minimum_required(VERSION 3.20)
project(DbgCensus VERSION 0.1.0 LANGUAGES CXX)

# Project-wide configuration
option(BUILD_TESTING "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

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

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(RUNTIME_DEPENDENCY_DIR ${CMAKE_INSTALL_BINDIR})
else()
    set(RUNTIME_DEPENDENCY_DIR ${CMAKE_INSTALL_LIBDIR})
endif()

install(RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}Deps
  PRE_EXCLUDE_REGEXES [[api-ms-win-.*]] [[ext-ms-.*]] [[kernel32\.dll]]
  POST_EXCLUDE_REGEXES [[.*/system32/.*\.dll]] [[.*/dbg_census/.+/.+\.dll]]
  DESTINATION ${RUNTIME_DEPENDENCY_DIR}
)

# TODO (@leonhard-s): Support packaging
