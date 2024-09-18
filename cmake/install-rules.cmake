install(
    TARGETS route_master_exe
    RUNTIME COMPONENT route_master_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
