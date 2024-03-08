add_library(${PROJECT_NAME}_Stream)
add_library(${PROJECT_NAME}::Stream ALIAS ${PROJECT_NAME}_Stream)

set_target_properties(${PROJECT_NAME}_Stream PROPERTIES
  OUTPUT_NAME dbg_census_stream
  EXPORT_NAME Stream
)

include(GenerateExportHeader)
set(EXPORT_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/dbg_census/stream/export.h)
generate_export_header(${PROJECT_NAME}_Stream
  EXPORT_FILE_NAME ${EXPORT_HEADER}
  EXPORT_MACRO_NAME DBGCENSUS_API
)

target_include_directories(${PROJECT_NAME}_Stream
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PRIVATE
    src
)

target_link_libraries(${PROJECT_NAME}_Stream
  PUBLIC
    ${PROJECT_NAME}::Common
  PRIVATE
    ixwebsocket::ixwebsocket
)

target_sources(${PROJECT_NAME}_Stream PRIVATE
  "include/dbg_census/stream/ess_client.h"
  "include/dbg_census/stream/subscription_builder.h"
  "src/ess_client.cpp"
  "src/subscription_builder.cpp"
  "${EXPORT_HEADER}"
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

set_property(TARGET ${PROJECT_NAME}_Stream PROPERTY
  PUBLIC_HEADER
    "include/dbg_census/stream/ess_client.h"
    "include/dbg_census/stream/subscription_builder.h"
    "${EXPORT_HEADER}"
)

include(GNUInstallDirs)
string(APPEND CMAKE_INSTALL_INCLUDEDIR /dbg_census/stream)
string(APPEND CMAKE_INSTALL_BINDIR /$<CONFIG>)
string(APPEND CMAKE_INSTALL_LIBDIR /$<CONFIG>)
install(TARGETS ${PROJECT_NAME}_Stream
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}Deps
)

if(BUILD_SHARED_LIBS AND WIN32)
  install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}_Stream> OPTIONAL
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()
