add_library(${PROJECT_NAME}_REST)
add_library(${PROJECT_NAME}::REST ALIAS ${PROJECT_NAME}_REST)

target_include_directories(${PROJECT_NAME}_REST
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(${PROJECT_NAME}_REST PRIVATE
    "include/dbg_census/rest/retry_strategy.h"
    "src/retry_strategy.cpp"
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
