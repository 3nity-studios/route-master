# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(route-master-tests LANGUAGES CXX)

include_directories(${CMAKE_BINARY_DIR}/source)

# ---- Tests ----

add_executable(route-master_test src/route-master_test.cpp)
target_link_libraries(
    route-master_test PRIVATE
    route-master_platform
    route-master_config
    route-master_utilities
    route-master_engine
    route-master_states
    nlohmann_json::nlohmann_json
    Designar
    Catch2::Catch2WithMain
)
target_compile_features(route-master_test PRIVATE cxx_std_17)

# add_test(NAME route-master_test COMMAND route-master_test)

catch_discover_tests(route-master_test)

# ---- End-of-file commands ----

add_folders(Test)
