#ifndef UTIL_IPLATFORM_HELPER_HPP
#define UTIL_IPLATFORM_HELPER_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowHandle.hpp>
namespace util
{
struct IPlatform
{
    virtual ~IPlatform() = default;
    virtual void set_icon(const sf::WindowHandle &in_handle) = 0;
    virtual void toggle_fullscreen(const sf::WindowHandle &in_handle, const sf::Uint32 in_style, const bool in_windowed,
                                   const sf::Vector2u &in_resolution) = 0;
    virtual int get_refresh_rate(const sf::WindowHandle &in_handle) = 0;
    virtual float get_screen_scaling_factor(const sf::WindowHandle &in_handle) = 0;
};
} // namespace util

#endif // UTIL_IPLATFORM_HELPER_HPP

// Macros
#define UNUSED(x) (void)(x)
