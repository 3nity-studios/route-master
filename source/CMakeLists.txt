cmake_minimum_required(VERSION 3.22.1)

# I don't have the slightest idea what this does. But it doesn't hurt c:
include(${PROJECT_SOURCE_DIR}/cmake/folders.cmake)

# ---- Set options ----

set(RM_REQUIRED_CXX "cxx_std_17") # also this should be in CMakePresets.json
set(RM_OS_LINUX TRUE)
# we are working on porting a macro that detects the OS, Linux is the default
# for now.

# ---- Declare modules ----

include_directories(${PROJECT_SOURCE_DIR}/source)
include_directories(${PROJECT_SOURCE_DIR}/source/include)
include_directories(${PROJECT_BINARY_DIR}/source)

# config

add_subdirectory(config)

# engine

add_subdirectory(engine)

# gui

add_subdirectory(states)

# platform

add_subdirectory(platform)

# utilities

add_subdirectory(utilities)

# ---- Declare source files ----

# set(INCROOT ${PROJECT_SOURCE_DIR}/source)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source)

# One could argue that a GLOB might suit us better
set(
  SRC
  ${SRCROOT}/main.cpp
)
source_group("" FILES ${SRC})

# ---- Declare executable ----

add_executable(route-master_exe ${SRC})
add_executable(route-master::exe ALIAS route-master_exe)

set_property(TARGET route-master_exe PROPERTY OUTPUT_NAME route-master)

target_compile_features(route-master_exe PRIVATE ${RM_REQUIRED_CXX})

target_link_libraries(
  route-master_exe
  PRIVATE
  route-master_platform
  route-master_config
  route-master_utilities
  route-master_engine
  route-master_states
  nlohmann_json::nlohmann_json
  Designar
  ${TMXLITE_LIBRARY_RELEASE}
  sfml-graphics
)
# TODO: Add support for TMXLITE_DEBUG_RELEASE

# Note: SFML does not properly propagate its requirements and it requires
#       OpenGL in its install interface when compiled as a static library.
if(SFML_STATIC_LIBRARIES AND NOT IOS AND NOT WIN32)
  find_package(OpenGL REQUIRED)
  target_link_libraries(route-master_exe PRIVATE OpenGL::GL)
endif()

# ---- End-of-file commands ----

add_folders(Source)
