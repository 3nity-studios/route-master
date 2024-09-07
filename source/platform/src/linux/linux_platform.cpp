#ifdef __linux__
#include "platform/include/linux/linux_platform.hpp"

#include <X11/Xlib.h>

namespace util
{
/******************************************************************************
 *
 *****************************************************************************/
LinuxPlatform::LinuxPlatform()
{
    // TODO: Investigate further
    XInitThreads();
}

/******************************************************************************
 *
 *****************************************************************************/
void LinuxPlatform::set_icon(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);
}

/******************************************************************************
 *
 *****************************************************************************/
void LinuxPlatform::toggle_fullscreen(const sf::WindowHandle &in_handle, const sf::Uint32 in_style,
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
float LinuxPlatform::get_screen_scaling_factor(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);
    return 1.0f;
}

/******************************************************************************
 *
 *****************************************************************************/
int LinuxPlatform::get_refresh_rate(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);
    return 59;
}
} // namespace util

#endif // __linux__
