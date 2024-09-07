block(SCOPE_FOR VARIABLES)

# disable warnings, tools and flags so the build process doesn't crumble down
unset(CMAKE_CXX_CPPCHECK)
set(CMAKE_CXX_FLAGS "-w")
unset(CMAKE_EXE_LINKER_FLAGS)
unset(CMAKE_SHARED_LINKER_FLAGS)
unset(CMAKE_CXX_CLANG_TIDY)
unset(CXX_CLANG_TIDY)
set(CMAKE_BUILD_TYPE "Release")

include(FetchContent)

# avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24+:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Don't fetch libraries everytime CMake runs")
set(FETCHCONTENT_QUIET FALSE)

set(BUILD_SHARED_LIBS OFF)

if(RM_USE_SYSTEM_JSON)
  find_package(nlohmann_json 3.11.3 REQUIRED)
else()
  FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(json)
endif()

if(RM_USE_SYSTEM_DESIGNAR) # one can only dream huh? c:
  find_package(Designar 2.0.1 REQUIRED)
else()
  FetchContent_Declare(
    Designar
    GIT_REPOSITORY https://github.com/3nity-studios/DeSiGNAR.git
    GIT_TAG        v2.0.1
    GIT_SHALLOW    ON
    GIT_PROGRESS TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(Designar)
endif()

if(RM_USE_SYSTEM_SFML)
  find_package(
    SFML 2.6
    COMPONENTS system window graphics audio
    REQUIRED
  )
else()
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY   https://github.com/SFML/SFML.git
    GIT_TAG          2.6.x
    GIT_SHALLOW      ON
    GIT_PROGRESS     TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(SFML)
endif()

# avoid error about CMake compatibility being dropped in the future
# as TMXLite CMakeLists.txt specifies an older CMake version (< 3.5)
set(CMAKE_ERROR_DEPRECATED OFF CACHE BOOL "" FORCE)

# to find TMXLite properly through FindTMXLITE.cmake as it doesn't provide a
# tmxliteConfig.cmake to keep up with modern CMake standards
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# TMXLite doesn't build as a static lib by default so this might prevent future
# linking errors
# set(TMXLITE_STATIC_LIB ON CACHE BOOL "")

if(RM_USE_SYSTEM_TMXLITE)
  find_package(
    TMXLITE 1.4
    REQUIRED
  )
else()
  FetchContent_Declare(
    tmxlite
    GIT_REPOSITORY   https://github.com/fallahn/tmxlite.git
    GIT_TAG          v1.4.4
    GIT_SHALLOW      ON
    GIT_PROGRESS     TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
    # PATCH_COMMAND    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_LIST_DIR}/TMXLITE_CMakeLists.txt" "${CMAKE_CURRENT_BINARY_DIR}/_deps/tmxlite-src/tmxlite/CMakeLists.txt"
  )
  FetchContent_MakeAvailable(tmxlite)
  add_subdirectory(${tmxlite_SOURCE_DIR}/tmxlite SYSTEM)
  set(TMXLITE_LIBRARY_RELEASE tmxlite CACHE STRING "")
  set(TMXLITE_INCLUDE_DIR ${tmxlite_SOURCE_DIR}/tmxlite/include CACHE STRING "")
  include_directories(${TMXLITE_INCLUDE_DIR} SYSTEM)
endif()

if(RM_DEVELOPER_MODE)
  if(RM_USE_SYSTEM_CATCH2)
    find_package(
      Catch2 3.7
      REQUIRED
    )
  else()
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY   https://github.com/catchorg/Catch2.git
      GIT_TAG          v3.7.0
      GIT_SHALLOW      ON
      GIT_PROGRESS     TRUE
      EXCLUDE_FROM_ALL
      SYSTEM
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
  endif()
  include(Catch)
endif()

endblock()
