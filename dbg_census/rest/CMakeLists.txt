add_library(${PROJECT_NAME}_REST)
add_library(${PROJECT_NAME}::REST ALIAS ${PROJECT_NAME}_REST)

option(HTTPS_SUPPORT "HTTPS support" ON)
if(HTTPS_SUPPORT AND NOT TARGET OpenSSL::SSL)
  message(WARNING "HTTPS support is enabled but OpenSSL is not found. Disabling HTTPS support.")
  set(HTTPS_SUPPORT OFF)
endif()

target_include_directories(${PROJECT_NAME}_REST
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${PROJECT_NAME}_REST
  PRIVATE
    $<$<BOOL:${HTTPS_SUPPORT}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

target_link_libraries(${PROJECT_NAME}_REST
  PRIVATE
    httplib::httplib
    $<$<BOOL:${HTTPS_SUPPORT}>:OpenSSL::SSL OpenSSL::Crypto>
)

target_sources(${PROJECT_NAME}_REST PRIVATE
    "include/dbg_census/rest/rest_api_client.h"
    "include/dbg_census/rest/retry_strategy.h"
    "src/http_client_adapter.cpp"
    "src/http_client_adapter.h"
    "src/rest_api_client.cpp"
    "src/retry_strategy.cpp"
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
