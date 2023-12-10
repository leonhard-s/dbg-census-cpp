add_library(${PROJECT_NAME}_REST)
add_library(${PROJECT_NAME}::REST ALIAS ${PROJECT_NAME}_REST)

set_target_properties(${PROJECT_NAME}_REST PROPERTIES
  EXPORT_NAME REST
  OUTPUT_NAME dbg_census_rest
)

option(HTTPS_SUPPORT "HTTPS support" ON)
if(HTTPS_SUPPORT AND NOT TARGET OpenSSL::SSL)
  message(WARNING "HTTPS support is enabled but OpenSSL is not found. Disabling HTTPS support.")
  set(HTTPS_SUPPORT OFF)
endif()

include(GenerateExportHeader)
set(EXPORT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/dbg_census/rest/export.h)
generate_export_header(${PROJECT_NAME}_REST
  EXPORT_FILE_NAME ${EXPORT_HEADER}
  EXPORT_MACRO_NAME DBGCENSUS_API
)

target_include_directories(${PROJECT_NAME}_REST
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE src
)

target_link_libraries(${PROJECT_NAME}_REST
  PUBLIC
    ${PROJECT_NAME}::Common
  PRIVATE
    ixwebsocket::ixwebsocket
    $<$<BOOL:${HTTPS_SUPPORT}>:OpenSSL::SSL OpenSSL::Crypto>
)

target_sources(${PROJECT_NAME}_REST PRIVATE
    "include/dbg_census/rest/http_status.h"
    "include/dbg_census/rest/rest_api_client.h"
    "include/dbg_census/rest/retry_strategy.h"
    "src/http_client_adapter.cpp"
    "src/http_client_adapter.h"
    "src/rest_api_client.cpp"
    "src/retry_strategy.cpp"
    "src/url_splitter.cpp"
    "src/url_splitter.h"
    "${EXPORT_HEADER}"
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

set_property(TARGET ${PROJECT_NAME}_REST PROPERTY
  PUBLIC_HEADER
    include/dbg_census/rest/http_status.h
    include/dbg_census/rest/rest_api_client.h
    include/dbg_census/rest/retry_strategy.h
    ${EXPORT_HEADER}
)

include(GNUInstallDirs)
string(APPEND CMAKE_INSTALL_BINDIR /$<CONFIG>)
string(APPEND CMAKE_INSTALL_LIBDIR /$<CONFIG>)
install(TARGETS ${PROJECT_NAME}_REST
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}Deps
)

if(BUILD_SHARED_LIBS AND WIN32)
  install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}_REST> OPTIONAL
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()
