#ifdef _WIN32
#include "platform/win32/windows_platform.hpp"
#include <winuser.h>
#include "platform/win32/resource.h"

namespace util
{
/******************************************************************************
 *
 *****************************************************************************/
WindowsPlatform::WindowsPlatform()
{
    // Get the default device info
    m_screenScalingFactor = get_screen_scaling_factor(nullptr);
    m_refreshRate = get_refresh_rate(nullptr);
}

/******************************************************************************
 * The window handle uses 32x32 (ICON_BIG) & 16x16 (ICON_SMALL) sized icons.
 * This should be called any time the SFML window is create/recreated
 *****************************************************************************/
void WindowsPlatform::set_icon(const sf::WindowHandle &in_handle)
{
    // Get the icon directory
    PBYTE iconDirectory = get_icon_directory(WIN32_ICON_MAIN);
    std::array<int, 5> icons = {16, 32, 48, 64, 128};

    std::size_t indexSmallIcon = static_cast<std::size_t>(
        std::min(std::max(std::ceil(m_screenScalingFactor - 1.0f), 0.0f), static_cast<float>(icons.size()) - 1.0f));
    std::size_t indexBigIcon = static_cast<std::size_t>(std::min(
        std::max(std::ceil(m_screenScalingFactor - 1.0f), 0.0f) + 1.0f, static_cast<float>(icons.size()) - 1.0f));

    HICON smallIcon = get_icon_from_icon_directory(iconDirectory, icons[indexSmallIcon]);
    HICON bigIcon = get_icon_from_icon_directory(iconDirectory, icons[indexBigIcon]);

    if (smallIcon)
        SendMessage(in_handle, WM_SETICON, ICON_SMALL, (LPARAM)smallIcon);

    if (bigIcon)
        SendMessage(in_handle, WM_SETICON, ICON_BIG, (LPARAM)bigIcon);

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroyicon
    // It is only necessary to call DestroyIcon for icons and cursors created with the following functions:
    //   CreateIconFromResourceEx (if called without the LR_SHARED flag)
}

/******************************************************************************
 *
 *****************************************************************************/
void WindowsPlatform::toggle_fullscreen(const sf::WindowHandle &in_handle, const std::uint32_t inStyle,
                                        const bool inWindowed, const sf::Vector2u &inResolution)
{
    DWORD win32Style = sfml_window_style_to_win3(inStyle);
    unsigned int flags = SWP_DRAWFRAME | SWP_FRAMECHANGED;

    if (inWindowed)
    {
        // Window (centered on the focused screen)
        HDC screenDC = GetDC(in_handle);
        int screenWidth = GetDeviceCaps(screenDC, HORZRES);
        int screenHeight = GetDeviceCaps(screenDC, VERTRES);
        ReleaseDC(in_handle, screenDC);

        int width = static_cast<int>(inResolution.x);
        int height = static_cast<int>(inResolution.y);
        int left = (screenWidth - width) / 2;
        int top = (screenHeight - height) / 2;
        RECT rectangle = {0, 0, width, height};
        AdjustWindowRect(&rectangle, win32Style, false);
        width = rectangle.right - rectangle.left;
        height = rectangle.bottom - rectangle.top;

        SetWindowLongPtr(in_handle, GWL_STYLE, win32Style);
        SetWindowLongPtr(in_handle, GWL_EXSTYLE, 0);
        SetWindowPos(in_handle, nullptr, left, top, width, height, flags);
    }
    else
    {
        // Fullscreen
        int width = static_cast<int>(inResolution.x);
        int height = static_cast<int>(inResolution.y);

        // first time prevents the border from showing in the corner
        SetWindowPos(in_handle, HWND_TOP, 0, 0, width, height, flags);
        SetWindowLongPtr(in_handle, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowLongPtr(in_handle, GWL_STYLE, win32Style);

        // second time cleans up the rect after the border has been removed
        SetWindowPos(in_handle, HWND_TOP, 0, 0, width, height, flags);

        // note: double SetWindowPos call isn't very effective on slower machines anyway :/
    }
    ShowWindow(in_handle, SW_SHOW);
}

/******************************************************************************
 * Gets the screen scaling factor of the device from the supplied handle
 *****************************************************************************/
float WindowsPlatform::get_screen_scaling_factor(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);

    if (m_screenScalingFactor != 0.0f)
        return m_screenScalingFactor;

    HDC screenDC = GetDC(nullptr);
    int logicalScreenHeight = GetDeviceCaps(screenDC, VERTRES);
    int physicalScreenHeight = GetDeviceCaps(screenDC, DESKTOPVERTRES);
    m_screenScalingFactor = static_cast<float>(physicalScreenHeight) / static_cast<float>(logicalScreenHeight);
    ReleaseDC(nullptr, screenDC);

    return m_screenScalingFactor;
}

/******************************************************************************
 * Gets the refresh rate of the device from the supplied handle
 *****************************************************************************/
int WindowsPlatform::get_refresh_rate(const sf::WindowHandle &in_handle)
{
    UNUSED(in_handle);

    if (m_refreshRate != 0)
        return m_refreshRate;

    HDC screenDC = GetDC(nullptr);
    m_refreshRate = GetDeviceCaps(screenDC, VREFRESH);
    ReleaseDC(nullptr, screenDC);

    return m_refreshRate;
}

/******************************************************************************
 * Loads a .ico file from The application's resources, and can contain multiple
 * sizes (for instance 16x16, 32x32 & 64x64). This is referred to as an
 * "Icon Directory". Additionally, it can have a single icon
 *****************************************************************************/
PBYTE WindowsPlatform::get_icon_directory(const int inResourceId)
{
    HMODULE hModule = GetModuleHandle(nullptr);
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(inResourceId), RT_GROUP_ICON);

    HGLOBAL hData = LoadResource(hModule, hResource);
    PBYTE data = (PBYTE)LockResource(hData);

    return data;
}

/******************************************************************************
 * This will attempt to load a single icon from an icon directory
 * If the requested size isn't found, the first one is returned
 *****************************************************************************/
HICON WindowsPlatform::get_icon_from_icon_directory(PBYTE inIconDirectory, const std::uint8_t inSize)
{
    HMODULE hModule = GetModuleHandle(nullptr);
    int resourceId = LookupIconIdFromDirectoryEx(inIconDirectory, TRUE, inSize, inSize, LR_DEFAULTCOLOR | LR_SHARED);
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), RT_ICON);

    HGLOBAL hData = LoadResource(hModule, hResource);
    PBYTE data = (PBYTE)LockResource(hData);
    DWORD sizeofData = SizeofResource(hModule, hResource);

    HICON icon =
        CreateIconFromResourceEx(data, sizeofData, TRUE, 0x00030000, inSize, inSize, LR_DEFAULTCOLOR | LR_SHARED);
    return icon;
}

/******************************************************************************
 * Takes an SFML window style and matches it back to the Win32 equivalent
 *****************************************************************************/
DWORD WindowsPlatform::sfml_window_style_to_win3(const std::uint32_t inStyle)
{
    DWORD style = 0;
    if (inStyle == sf::Style::None)
    {
        style = WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }
    else
    {
        style = WS_VISIBLE;
        if (inStyle & sf::Style::Titlebar)
            style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (inStyle & sf::Style::Resize)
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (inStyle & sf::Style::Close)
            style |= WS_SYSMENU;
    }

    return style;
}
} // namespace util

#endif // _WIN32
