# platform

set(INCROOT ${PROJECT_SOURCE_DIR}/source/platform/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/source/platform/src)

set(RM_PLATFORM_EXTRA_LIBRARIES "")

if(RM_OS_LINUX)
  set(
    SRC
    ${INCROOT}/linux/linux_platform.hpp
    ${SRCROOT}/linux/linux_platform.cpp
  )
  source_group("" FILES ${SRC})
  find_package(X11 REQUIRED)
  set(RM_PLATFORM_EXTRA_LIBRARIES "${X11_LIBRARIES}")
elseif(RM_OS_WIN32)
  set(
    SRC
    ${INCROOT}/win32/resource.h
    ${SRCROOT}/win32/resource.rc
    ${INCROOT}/win32/windows_platform.hpp
    ${SRCROOT}/win32/windows_platform.cpp
  )
  source_group("" FILES ${SRC})
elseif(RM_OS_MACOS)
  set(
    SRC
    ${INCROOT}/macos/macos_platform.hpp
    ${SRCROOT}/macos/macos_platform.cpp
  )
  source_group("" FILES ${SRC})
endif()

add_library(
  route-master_platform OBJECT
  ${SRC}
  ${INCROOT}/iplatform.hpp
  ${INCROOT}/platform.hpp
)

target_include_directories(
  route-master_platform ${warning_guard}
  PRIVATE
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/platform/include>"
)

target_compile_features(route-master_platform PRIVATE ${RM_REQUIRED_CXX})

target_link_libraries(
  route-master_platform
  PRIVATE
  sfml-window
  sfml-system
  ${RM_PLATFORM_EXTRA_LIBRARIES}
)
