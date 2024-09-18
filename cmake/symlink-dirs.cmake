function(symlink_common_dirs target)
  ### resources
  set(GAME_BIN_DIR "$<TARGET_FILE_DIR:${target}>")

  # Symlink resources
  set(GAME_RES_DIR "${PROJECT_SOURCE_DIR}/assets")
  if(NOT EXISTS "${GAME_BIN_DIR}/assets")
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_RES_DIR}" "${GAME_BIN_DIR}/assets"
      VERBATIM
    )
  endif()
endfunction()
