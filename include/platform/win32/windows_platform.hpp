#ifndef UTIL_WINDOWS_PLATFORM_HPP
#define UTIL_WINDOWS_PLATFORM_HPP

#include <Windows.h>
#include <array>
#include "platform/iplatform.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <winuser.h>
#include <cmath>
#include <cstdint>

// TODO: WM_DISPLAYCHANGE event handling (multi-monitor support)

namespace util
{
struct WindowsPlatform : IPlatform
{
    WindowsPlatform();

    void set_icon(const sf::WindowHandle &in_handle) final;
    void toggle_fullscreen(const sf::WindowHandle &in_handle, const std::uint32_t in_style, const bool in_windowed,
                           const sf::Vector2u &in_resolution) final;
    float get_screen_scaling_factor(const sf::WindowHandle &in_handle) final;
    int get_refresh_rate(const sf::WindowHandle &in_handle) final;

  private:
    PBYTE get_icon_directory(const int inResourceId);
    HICON get_icon_from_icon_directory(PBYTE inIconDirectory, const std::uint8_t inSize);
    DWORD sfml_window_style_to_win3(const std::uint32_t inStyle);

    float m_screenScalingFactor = 0.0f;
    int m_refreshRate = 0;
};
} // namespace util

#endif // UTIL_WINDOWS_PLATFORM_HPP
