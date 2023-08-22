add_library(${PROJECT_NAME}_Stream)
add_library(${PROJECT_NAME}::Stream ALIAS ${PROJECT_NAME}_Stream)

set_target_properties(${PROJECT_NAME}_Stream PROPERTIES
  OUTPUT_NAME Stream
  EXPORT_NAME dbg_census_stream
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
  PRIVATE
    ixwebsocket::ixwebsocket
)

target_sources(${PROJECT_NAME}_Stream PRIVATE
  "include/dbg_census/stream/ess_client.h"
  "src/ess_client.cpp"
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_Stream EXPORT ${PROJECT_NAME}Targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${EXPORT_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dbg_census/stream)

if(BUILD_SHARED_LIBS AND WIN32)
  install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}_Stream> OPTIONAL
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()
