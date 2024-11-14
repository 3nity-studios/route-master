cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
include(cmake/prelude.cmake)

# avoid warning about deduplicating libraries when linking in CMake 3.29+:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.29.0")
  cmake_policy(SET CMP0156 NEW)
endif()

# ---- Export compile commands for completion engines ----

set(CMAKE_EXPORT_COMPILE_COMMANDS=TRUE)

# ---- Set directory for the resulting binaries ----

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---- Set project version and details ----

project(
    route-master
    VERSION 0.0.1
    DESCRIPTION "Route Master by Trinity Studios"
    HOMEPAGE_URL "https://github.com/3nity-studios/route-master"
    LANGUAGES CXX
)

include(cmake/variables.cmake)
include(cmake/folders.cmake)
include(cmake/symlink-dirs.cmake)

# ---- Configurable options ----

option(RM_STATIC_LINK "Link all libraries statically to produce a single-file executable" ON)
option(RM_USE_SYSTEM_JSON "Use system's installation of nlohmann's JSON library" OFF)
option(RM_USE_SYSTEM_DESIGNAR "Use system's installation of DeSiGNAR library" OFF)
option(RM_USE_SYSTEM_SFML "Use system's installation of SFML library" OFF)
option(RM_USE_SYSTEM_CATCH2 "Use system's installation of Catch2 library" OFF)
option(RM_USE_SYSTEM_TMXLITE "Use system's installation of TMXLITE library" OFF)
option(RM_USE_SYSTEM_TGUI "Use system's installation of TGUI library" OFF)

option(USE_INSTALL_RESOURCE_PATH "Set resource path to install location" OFF)
if(NOT USE_INSTALL_RESOURCE_PATH)
  set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/assets/")
else()
  set(RESOURCE_PATH 
    "${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}/assets/")
endif()

# ---- Third Party Dependencies ----

include(cmake/third-party.cmake)

# ---- Build source code ----

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source)

include_directories(${SRCROOT})
include_directories(${INCROOT})
include_directories(${CMAKE_BINARY_DIR}/include)

# ---- config

# Generate config.hpp
configure_file(${INCROOT}/config/config.hpp.in include/config/config.hpp)

set(
  RM_CONFIG_SRC
  ${INCROOT}/config/global.hpp
  ${INCROOT}/config/game.hpp
  ${SRCROOT}/config/game.cpp
  # ${INCROOT}/config.hpp
  ${SRCROOT}/config/config.cpp
)
add_library(route_master_config ${RM_CONFIG_SRC})
add_library(route_master::config ALIAS route_master_config)
target_link_libraries(route_master_config
  PRIVATE
  TGUI::TGUI
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  Designar::Designar
  tmxlite::tmxlite
  nlohmann_json::nlohmann_json
  route_master::states)

# ---- engine

set(
  RM_ENGINE_SRC
  ${SRCROOT}/engine/asset_manager.cpp
  ${INCROOT}/engine/asset_manager.hpp
  ${SRCROOT}/engine/input_manager.cpp
  ${INCROOT}/engine/input_manager.hpp
  ${SRCROOT}/engine/state_machine.cpp
  ${INCROOT}/engine/state_machine.hpp
  ${INCROOT}/engine/tmx_core.hpp
)
add_library(route_master_engine ${RM_ENGINE_SRC})
add_library(route_master::engine ALIAS route_master_engine)
target_link_libraries(route_master_engine
  PRIVATE
  TGUI::TGUI
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  Designar::Designar
  tmxlite::tmxlite
  nlohmann_json::nlohmann_json)

# ---- simulation

set(
  RM_SIMULATION_SRC
  ${INCROOT}/simulation/VisualElement.hpp
  ${INCROOT}/simulation/Bus.hpp
  ${INCROOT}/simulation/BusStop.hpp
  ${INCROOT}/simulation/TrafficLight.hpp
  ${INCROOT}/simulation/City.hpp
  ${INCROOT}/simulation/Route.hpp
  ${INCROOT}/simulation/SimulationInfo.hpp
  ${INCROOT}/simulation/Employee.hpp
  ${INCROOT}/simulation/Passenger.hpp
  ${INCROOT}/simulation/Street.hpp
  ${SRCROOT}/simulation/VisualElement.cpp
  ${SRCROOT}/simulation/Bus.cpp
  ${SRCROOT}/simulation/BusStop.cpp
  ${SRCROOT}/simulation/TrafficLight.cpp
  ${SRCROOT}/simulation/City.cpp
  ${SRCROOT}/simulation/Route.cpp
  ${SRCROOT}/simulation/SimulationInfo.cpp
  ${SRCROOT}/simulation/Employee.cpp
  ${SRCROOT}/simulation/Passenger.cpp
  ${SRCROOT}/simulation/Street.cpp
)
add_library(route_master_simulation ${RM_SIMULATION_SRC})
add_library(route_master::simulation ALIAS route_master_simulation)
target_link_libraries(route_master_simulation
  PRIVATE
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  TGUI::TGUI
  Designar::Designar
  tmxlite::tmxlite
  nlohmann_json::nlohmann_json)

# ---- player

set(
  RM_PLAYER_SRC
  ${INCROOT}/player/Player.hpp
  ${INCROOT}/player/Store.hpp
  ${INCROOT}/player/Item.hpp
  ${INCROOT}/player/Achievement.hpp
  ${INCROOT}/player/AchievementManager.hpp
  ${SRCROOT}/player/Player.cpp
  ${SRCROOT}/player/Store.cpp
  ${SRCROOT}/player/Item.cpp
  ${SRCROOT}/player/Achievement.cpp
  ${SRCROOT}/player/AchievementManager.cpp
)
add_library(route_master_player ${RM_PLAYER_SRC})
add_library(route_master::player ALIAS route_master_player)
target_link_libraries(route_master_player
  PRIVATE
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  TGUI::TGUI
  Designar::Designar
  nlohmann_json::nlohmann_json
  route_master::simulation)

# ---- states

set(
  RM_STATES_SRC
  ${INCROOT}/states/state.hpp
  ${INCROOT}/states/main_menu_state.hpp
  ${SRCROOT}/states/main_menu_state.cpp
  ${INCROOT}/states/store_state.hpp
  ${SRCROOT}/states/store_state.cpp
  ${INCROOT}/states/map_viewer_state.hpp
  ${SRCROOT}/states/map_viewer_state.cpp
  ${INCROOT}/states/simulation_state.hpp
  ${SRCROOT}/states/simulation_state.cpp
  ${INCROOT}/states/bus_maintenance_state.hpp
  ${SRCROOT}/states/bus_maintenance_state.cpp
  ${INCROOT}/states/bus_improvements_state.hpp
  ${SRCROOT}/states/bus_improvements_state.cpp
  ${INCROOT}/states/bus_select_state.hpp
  ${SRCROOT}/states/bus_select_state.cpp
  ${INCROOT}/states/route_edit_state.hpp
  ${SRCROOT}/states/route_edit_state.cpp
  ${INCROOT}/states/route_list_state.hpp
  ${SRCROOT}/states/route_list_state.cpp
  ${INCROOT}/states/management_state.hpp
  ${SRCROOT}/states/management_state.cpp
  ${INCROOT}/states/stats_state.hpp
  ${SRCROOT}/states/stats_state.cpp
  ${INCROOT}/states/achievements_state.hpp
  ${SRCROOT}/states/achievements_state.cpp
)
add_library(route_master_states ${RM_STATES_SRC})
add_library(route_master::states ALIAS route_master_states)
target_link_libraries(route_master_states
  PRIVATE
  TGUI::TGUI
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  Designar::Designar
  tmxlite::tmxlite
  nlohmann_json::nlohmann_json
  route_master::simulation
  route_master::player
  route_master::engine)

# ---- platform

set(RM_PLATFORM_EXTRA_LIBRARIES "")

if(CMAKE_SYSTEM MATCHES "Linux")
  set(
    RM_PLATFORM_SPECIFIC
    ${INCROOT}/platform/linux/linux_platform.hpp
    ${SRCROOT}/platform/linux/linux_platform.cpp
  )
elseif(CMAKE_SYSTEM MATCHES "Windows")
  set(
    RM_PLATFORM_SPECIFIC
    ${INCROOT}/platform/win32/resource.h
    ${SRCROOT}/platform/win32/resource.rc
    ${INCROOT}/platform/win32/windows_platform.hpp
    ${SRCROOT}/platform/win32/windows_platform.cpp
  )
elseif(CMAKE_SYSTEM MATCHES "Darwin")
  set(
    RM_PLATFORM_SPECIFIC
    ${INCROOT}/platform/macos/macos_platform.hpp
    ${SRCROOT}/platform/macos/macos_platform.cpp
  )
endif()

set(
  RM_PLATFORM_GENERIC
  ${INCROOT}/platform/platform.hpp
)

set(
  RM_PLATFORM_SRC
  ${RM_PLATFORM_GENERIC}
  ${RM_PLATFORM_SPECIFIC}
)
add_library(route_master_platform ${RM_PLATFORM_SRC})
add_library(route_master::platform ALIAS route_master_platform)
target_link_libraries(route_master_platform
  PRIVATE
  TGUI::TGUI
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  Designar::Designar
  tmxlite::tmxlite
  nlohmann_json::nlohmann_json)

# ---- utils

set(
  RM_UTILS_SRC
  ${INCROOT}/utils/filesystem.hpp
  ${INCROOT}/utils/street_factory.hpp
  ${INCROOT}/utils/JSON.hpp
  ${SRCROOT}/utils/street_factory.cpp
  ${INCROOT}/utils/calc_view.hpp
  ${SRCROOT}/utils/calc_view.cpp
  ${INCROOT}/utils/view_dragger.hpp
  ${SRCROOT}/utils/view_dragger.cpp
  ${SRCROOT}/utils/JSON.cpp
)
add_library(route_master_utils ${RM_UTILS_SRC})
add_library(route_master::utils ALIAS route_master_utils)
target_link_libraries(route_master_utils
  PRIVATE
  TGUI::TGUI
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  Designar::Designar
  tmxlite::tmxlite
  nlohmann_json::nlohmann_json)

# ---- Group all modules

set(SRC
  # ${RM_CONFIG_SRC}
  # ${RM_ENGINE_SRC}
  # ${RM_STATES_SRC}
  # ${RM_PLATFORM_SRC}
  # ${RM_UTILS_SRC}
  ${SRCROOT}/main.cpp
)

# ---- Declare executable ----

add_executable(route_master_exe ${SRC})
add_executable(route_master::exe ALIAS route_master_exe)
set_property(TARGET route_master_exe PROPERTY OUTPUT_NAME route_master)

target_include_directories(
  route_master_exe
  PRIVATE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
)

target_link_libraries(
  route_master_exe
  PRIVATE
  route_master::states
  route_master::config
  route_master::platform
  route_master::engine
  route_master::simulation
  route_master::player
  route_master::utils
  TGUI::TGUI
  SFML::Graphics
  SFML::Window
  SFML::Audio
  SFML::System
  ##opengl32
  ##gdi32
  #freetype
  #FLAC
  #vorbisenc
  #vorbisfile
  #vorbis
  #ogg
  ##winmm
  nlohmann_json::nlohmann_json
  Designar::Designar
  tmxlite::tmxlite
  route_master::simulation
  ${RM_PLATFORM_EXTRA_LIBRARIES}
)

symlink_common_dirs(route_master::exe)

# Note: SFML does not properly propagate its requirements and it requires
#       OpenGL in its install interface when compiled as a static library.
if(SFML_STATIC_LIBRARIES AND NOT IOS AND NOT WIN32)
  find_package(OpenGL REQUIRED)
  target_link_libraries(route_master_exe PRIVATE OpenGL::GL)
endif()

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- End-of-file commands ----

add_folders(Project)

# ---- Developer mode ----

if(RM_DEVELOPER_MODE)
  include(cmake/dev-mode.cmake)
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of Route Master"
  )
endif()
