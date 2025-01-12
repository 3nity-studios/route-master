function(symlink_common_dirs target)
  ### resources
  set(GAME_BIN_DIR "$<TARGET_FILE_DIR:${target}>")

  # Symlink resources
  set(GAME_RES_DIR "${PROJECT_SOURCE_DIR}/assets")
  if(NOT EXISTS "${GAME_BIN_DIR}/assets")
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.25")
      add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_RES_DIR}" "${GAME_BIN_DIR}/assets"
        VERBATIM
      )
    else()
      # Because OUTPUT option may not use generator expressions,
      # extract name of file from target's properties.
      get_target_property(mytarget_basename ${target} OUTPUT_NAME)
      get_target_property(mytarget_suffix ${target} SUFFIX)
      set(mytarget_filename ${mytarget_basename}${mytarget_suffix})
      # make copied file be dependent from one which is build.
      # Note, that DEPENDS here creates dependencies both from the target
      # and from the file it creates.
      add_custom_command(OUTPUT
        ${CMAKE_BINARY_DIR}/${mytarget_filename} COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_RES_DIR}" "${GAME_BIN_DIR}/assets"
        VERBATIM
          DEPENDS ${target}
      )
      # Create target which consume the command via DEPENDS.
      add_custom_target(symlink_assets ALL
          DEPENDS ${CMAKE_BINARY_DIR}/${mytarget_filename}
      )
    endif()
  endif()
endfunction()
