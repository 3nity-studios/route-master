#ifndef UTIL_MACOS_PLATFORM_HPP
#define UTIL_MACOS_PLATFORM_HPP

#include "platform/iplatform.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowHandle.hpp>

namespace util
{
struct MacOSPlatform : IPlatform
{
    MacOSPlatform();

    void set_icon(const sf::WindowHandle &in_handle) final;
    void toggle_fullscreen(const sf::WindowHandle &in_handle, const sf::Uint32 in_style, const bool in_windowed,
                           const sf::Vector2u &in_resolution) final;
    float get_screen_scaling_factor(const sf::WindowHandle &in_handle) final;
    int get_refresh_rate(const sf::WindowHandle &in_handle) final;
};
} // namespace util

#endif // UTIL_MACOS_PLATFORM_HPP
