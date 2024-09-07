# engine

set(INCROOT ${PROJECT_SOURCE_DIR}/source/engine/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source/engine/src)

set(
  SRC
  ${SRCROOT}/asset_manager.cpp
  ${INCROOT}/asset_manager.hpp
  ${SRCROOT}/input_manager.cpp
  ${INCROOT}/input_manager.hpp
  ${SRCROOT}/state_machine.cpp
  ${INCROOT}/state_machine.hpp
)
source_group("" FILES ${SRC})

add_library(
  route-master_engine OBJECT
  ${SRC}
)

target_include_directories(
  route-master_engine ${warning_guard}
  PRIVATE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/engine/include>"
)

target_compile_features(route-master_engine PRIVATE ${RM_REQUIRED_CXX})

target_link_libraries(
  route-master_engine
  PRIVATE
  sfml-graphics
  sfml-window
  sfml-audio
)
