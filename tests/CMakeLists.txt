# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(route_master-tests LANGUAGES CXX)

include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/include)

# ---- Tests ----

add_executable(route_master_test src/route_master_test.cpp)
target_link_libraries(
    route_master_test PRIVATE
    route_master::states
    route_master::config
    route_master::platform
    route_master::engine
    route_master::utils
    tmxlite::tmxlite
    nlohmann_json::nlohmann_json
    Designar::Designar
    Catch2::Catch2WithMain
)
target_compile_features(route_master_test PRIVATE cxx_std_17)

set(SIMULATION_TESTS_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/src/simulation_tests.cpp
    ${CMAKE_SOURCE_DIR}/source/simulation/Bus.cpp
    ${CMAKE_SOURCE_DIR}/source/simulation/BusStop.cpp
    ${CMAKE_SOURCE_DIR}/source/simulation/City.cpp
    ${CMAKE_SOURCE_DIR}/source/simulation/Street.cpp
    ${CMAKE_SOURCE_DIR}/source/simulation/Employee.cpp
    ${CMAKE_SOURCE_DIR}/source/simulation/Passenger.cpp
)

add_executable(simulation_tests ${SIMULATION_TESTS_SOURCES})

target_include_directories(simulation_tests PRIVATE ${CMAKE_SOURCE_DIR}/include/simulation)

target_link_libraries(
    simulation_tests PRIVATE
    route_master::states
    route_master::config
    route_master::platform
    route_master::engine
    route_master::utils
    tmxlite::tmxlite
    nlohmann_json::nlohmann_json
    Designar::Designar
    Catch2::Catch2WithMain
)
target_compile_features(simulation_tests PRIVATE cxx_std_17)

set(PLAYER_TESTS_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/src/player_tests.cpp
    ${CMAKE_SOURCE_DIR}/source/player/Item.cpp
    ${CMAKE_SOURCE_DIR}/source/player/Player.cpp
    ${CMAKE_SOURCE_DIR}/source/player/Store.cpp
)

add_executable(player_tests ${PLAYER_TESTS_SOURCES})

target_include_directories(player_tests PRIVATE ${CMAKE_SOURCE_DIR}/include/player)

target_link_libraries(
    player_tests PRIVATE
    route_master::states
    route_master::config
    route_master::platform
    route_master::engine
    route_master::utils
    route_master::simulation
    tmxlite::tmxlite
    nlohmann_json::nlohmann_json
    Designar::Designar
    Catch2::Catch2WithMain
)
target_compile_features(player_tests PRIVATE cxx_std_17)

# add_test(NAME route_master_test COMMAND route_master_test)

catch_discover_tests(route_master_test)
catch_discover_tests(simulation_tests)
catch_discover_tests(player_tests)

# ---- End-of-file commands ----

add_folders(Test)
