#ifndef UTIL_PLATFORM_HPP
#define UTIL_PLATFORM_HPP

#if defined(__APPLE__)
#include "platform/macos/macos_platform.hpp"
#elif defined(__linux__)
#include "platform/linux/linux_platform.hpp"
#elif defined(_WIN32)
#include "platform/win32/windows_platform.hpp"
#endif

namespace util
{
#if defined(__APPLE__)
using Platform = MacOSPlatform;
#elif defined(__linux__)
using Platform = LinuxPlatform;
#elif defined(_WIN32)
using Platform = WindowsPlatform;
#endif
} // namespace util
#endif // UTIL_PLATFORM_HPP
