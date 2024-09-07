# states

set(INCROOT ${PROJECT_SOURCE_DIR}/source/states/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source/states/src)

set(
  SRC
  ${INCROOT}/state.hpp
  ${INCROOT}/main_menu_state.hpp
  ${SRCROOT}/main_menu_state.cpp
)
source_group("" FILES ${SRC})

add_library(
  route-master_states OBJECT
  ${SRC}
)

target_include_directories(
  route-master_states ${warning_guard}
  PRIVATE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/states/include>"
)

target_compile_features(route-master_states PRIVATE ${RM_REQUIRED_CXX})

target_link_libraries(
  route-master_states
  PRIVATE
  sfml-graphics
)
