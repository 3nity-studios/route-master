set(INCROOT ${PROJECT_SOURCE_DIR}/source/utilities/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source/utilities/src)

set(
  SRC
  ${INCROOT}/filesystem.hpp
)
source_group("" FILES ${SRC})

add_library(
  route-master_utilities OBJECT
  ${SRC}
)

target_include_directories(
  route-master_utilities ${warning_guard}
  PRIVATE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/utilities/include>"
)

target_compile_features(route-master_utilities PRIVATE ${RM_REQUIRED_CXX})
