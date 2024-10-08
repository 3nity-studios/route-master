#ifndef UTIL_LINUX_PLATFORM_HPP
#define UTIL_LINUX_PLATFORM_HPP

#include "platform/iplatform.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <cstdint>

namespace util
{
struct LinuxPlatform : IPlatform
{
    LinuxPlatform();

    void set_icon(const sf::WindowHandle &in_handle) final;
    void toggle_fullscreen(const sf::WindowHandle &in_handle, const std::uint32_t in_style, const bool in_windowed,
                           const sf::Vector2u &in_resolution) final;
    float get_screen_scaling_factor(const sf::WindowHandle &in_handle) final;
    int get_refresh_rate(const sf::WindowHandle &in_handle) final;
};
} // namespace util

#endif // UTIL_LINUX_PLATFORM_HPP
