install(
    TARGETS route-master_exe
    RUNTIME COMPONENT route-master_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
