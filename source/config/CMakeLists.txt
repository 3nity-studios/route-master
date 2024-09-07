#config

set(INCROOT ${PROJECT_SOURCE_DIR}/source/config/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source/config/src)

# Generate config.hpp
configure_file(${INCROOT}/config.hpp.in include/config.hpp)

include_directories(${PROJECT_BINARY_DIR}/source/config/include)

set(
  SRC
  ${INCROOT}/global.hpp
  ${INCROOT}/game.hpp
  ${SRCROOT}/game.cpp
  # ${INCROOT}/config.hpp
  ${SRCROOT}/config.cpp
)
source_group("" FILES ${SRC})

# ---- Declare executable ----

add_library(
  route-master_config OBJECT
  ${SRC}
)

target_include_directories(
  route-master_config ${warning_guard}
  PRIVATE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/config/include>"
)

target_compile_features(route-master_config PRIVATE ${RM_REQUIRED_CXX})

target_link_libraries(
  route-master_config
  PRIVATE
  sfml-graphics
)
