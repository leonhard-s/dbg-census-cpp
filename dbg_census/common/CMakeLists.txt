add_library(${PROJECT_NAME}_Common INTERFACE)
add_library(${PROJECT_NAME}::Common ALIAS ${PROJECT_NAME}_Common)

set_target_properties(${PROJECT_NAME}_Common PROPERTIES
  EXPORT_NAME Common
)

target_include_directories(${PROJECT_NAME}_Common
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_property(TARGET ${PROJECT_NAME}_Common PROPERTY
  PUBLIC_HEADER
    include/dbg_census/common/macros.h
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_Common
  EXPORT ${PROJECT_NAME}Targets
)
