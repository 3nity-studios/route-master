include(cmake/folders.cmake)

include(CTest)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_custom_target(
    run-exe
    COMMAND route_master_exe
    VERBATIM
)
add_dependencies(run-exe route_master_exe)

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
