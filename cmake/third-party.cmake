block(SCOPE_FOR VARIABLES)

# disable warnings, tools and flags so the build process doesn't crumble down
unset(CMAKE_CXX_CPPCHECK)
set(CMAKE_CXX_FLAGS "-w")
unset(CMAKE_EXE_LINKER_FLAGS)
unset(CMAKE_SHARED_LINKER_FLAGS)
unset(CMAKE_CXX_CLANG_TIDY)
unset(CXX_CLANG_TIDY)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS TRUE CACHE INTERNAL "Suppress dev warnings" FORCE)

set(CMAKE_BUILD_TYPE "Release")

include(FetchContent)

# avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24+:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Don't fetch libraries everytime CMake runs")
set(FETCHCONTENT_QUIET FALSE)
# this is to prevent redownloading dependencies each time build folders are
#  removed.
#  ccache would probably be a more elegant solution tho.
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/_deps CACHE PATH "Directory to download the dependencies to" FORCE)

if(RM_STATIC_LINK)
  set(BUILD_SHARED_LIBS OFF)

  set(SFML_STATIC_LIBRARIES TRUE)
  add_compile_definitions(SFML_STATIC)
  
  option(TMXLITE_STATIC_LIBRARIES "Search for tmxlite static dependencies" ON)
  if(TMXLITE_STATIC_LIBRARIES)
    add_compile_definitions(TMXLITE_STATIC)
  endif()
  set(TMXLITE_FETCH_EXTLIBS TRUE CACHE INTERNAL "Automatically download TMXLITE dependencies")

  set(DESIGNAR_STATIC_LIBRARIES TRUE)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

if(RM_USE_SYSTEM_JSON)
  find_package(nlohmann_json 3.11.3 REQUIRED)
else()
  FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
    TLS_VERIFY TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(json)
endif()

if(RM_USE_SYSTEM_DESIGNAR)
  find_package(Designar 2.0.1 REQUIRED)
else()
  FetchContent_Declare(
    Designar
    GIT_REPOSITORY https://github.com/3nity-studios/DeSiGNAR.git
    GIT_TAG        35295635b580fd0d7aedae4af31f5221652a0d37 # v2.0.1
    GIT_SHALLOW    ON
    GIT_PROGRESS TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(Designar)
endif()

if(RM_USE_SYSTEM_SFML)
  find_package(
    SFML 3
    COMPONENTS System Window Graphics Audio
    REQUIRED
  )
else()
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY   https://github.com/SFML/SFML.git
    GIT_TAG          2116a3ba85673e3e30dc637a2cf2e20d1c4aa710 # 3.0.0-rc.1
    GIT_SHALLOW      ON
    GIT_PROGRESS     TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(SFML)
endif()

if(RM_USE_SYSTEM_TMXLITE)
  find_package(
    tmxlite 1.4
    REQUIRED
  )
else()
  FetchContent_Declare(
    tmxlite
    GIT_REPOSITORY   https://github.com/3nity-studios/tmxlite.git
    GIT_TAG          b8794039176e3c95f1305f84958a515ef8b98e64 # v1.4.4.1
    GIT_SHALLOW      ON
    GIT_PROGRESS     TRUE
    SOURCE_SUBDIR    tmxlite
    EXCLUDE_FROM_ALL
    SYSTEM
  )
  FetchContent_MakeAvailable(tmxlite)
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
      GIT_TAG          31588bb4f56b638dd5afc28d3ebff9b9dcefb88d # v3.7.0
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
