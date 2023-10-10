add_library(${PROJECT_NAME}_UrlGen INTERFACE)
add_library(${PROJECT_NAME}::UrlGen ALIAS ${PROJECT_NAME}_UrlGen)

set_target_properties(${PROJECT_NAME}_UrlGen PROPERTIES
  EXPORT_NAME UrlGen
)

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

set_property(TARGET ${PROJECT_NAME}_UrlGen PROPERTY
  PUBLIC_HEADER
    "include/dbg_census/urlgen/url_builder.h"
)

install(TARGETS ${PROJECT_NAME}_UrlGen EXPORT ${PROJECT_NAME}Targets)
