include(GNUInstallDirs)

# Install executable
install(
    TARGETS route_master_exe
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT route_master_Runtime
)

# Install assets alongside installation
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/assets"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/route-master"
    COMPONENT route_master_Runtime
)

# On Linux, install desktop file and icons
if(UNIX AND NOT APPLE)
  if(EXISTS "${PROJECT_SOURCE_DIR}/res/linux/sfml.png")
    install(
        FILES "${PROJECT_SOURCE_DIR}/res/linux/sfml.png"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps"
        RENAME route-master.png
        COMPONENT route_master_Runtime
    )
  endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
  set(CPACK_PACKAGE_NAME "route-master")
  set(CPACK_PACKAGE_VENDOR "Trinity Studios")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Route Master - Bus Transit Simulation")
  set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/3nity-studios/route-master")
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

  if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Route Master")
    set(CPACK_NSIS_DISPLAY_NAME "Route Master")
    set(CPACK_NSIS_PACKAGE_NAME "Route Master")
    set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/res/windows/app.ico")
    set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/res/windows/app.ico")
    set(CPACK_NSIS_MODIFY_PATH OFF)
  else()
    set(CPACK_GENERATOR "TGZ;ZIP;DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Trinity Studios")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
  endif()

  include(CPack)
endif()
