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

# add_test(NAME route_master_test COMMAND route_master_test)

catch_discover_tests(route_master_test)

# ---- End-of-file commands ----

add_folders(Test)
