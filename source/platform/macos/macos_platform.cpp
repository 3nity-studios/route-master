#ifdef __APPLE__
#include "platform/macos/macos_platform.hpp"

#include <CoreFoundation/CoreFoundation.h>

namespace util
{
/******************************************************************************
 *
 *****************************************************************************/
MacOSPlatform::MacOSPlatform()
{
    // This function ensures the working directory is set inside of the bundle if in production mode
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    bool pathSet = CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX);

    // This is a copy, so we release it here
    CFRelease(resourcesURL);

    // Actually do the check here
    if (pathSet)
    {
        std::string pathStr = path;
        if (pathStr.find(".app") != std::string::npos)
            chdir(path);
    }
}

/******************************************************************************
 *
 *****************************************************************************/
void MacOSPlatform::set_icon(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);
}

/******************************************************************************
 *
 *****************************************************************************/
void MacOSPlatform::toggle_fullscreen(const sf::WindowHandle &in_handle, const sf::Uint32 in_style,
                                      const bool in_windowed, const sf::Vector2u &in_resolution)
{
    UNUSED(in_handle);
    UNUSED(in_style);
    UNUSED(in_windowed);
    UNUSED(in_resolution);
}

/******************************************************************************
 *
 *****************************************************************************/
float MacOSPlatform::get_screen_scaling_factor(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);
    return 1.0f;
}

/******************************************************************************
 *
 *****************************************************************************/
int MacOSPlatform::get_refresh_rate(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);
    return 59;
}
} // namespace util

#endif // __APPLE__
