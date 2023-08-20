add_library(${PROJECT_NAME}_UrlGen INTERFACE)
add_library(${PROJECT_NAME}::UrlGen ALIAS ${PROJECT_NAME}_UrlGen)

target_include_directories(${PROJECT_NAME}_UrlGen
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(${PROJECT_NAME}_UrlGen PRIVATE
    "include/dbg_census/urlgen/url_builder.h"
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
