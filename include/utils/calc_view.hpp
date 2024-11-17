#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace util
{

static void calc_view(sf::RenderWindow &game_window, sf::View &view)
{
    unsigned int x_size = game_window.getSize().x;
    unsigned int y_size = game_window.getSize().y;

    float x_float = static_cast<float>(x_size / game_window.getSize().x);
    float y_float = static_cast<float>(y_size / game_window.getSize().y);
    auto window_size = sf::FloatRect({0.f, 0.f}, {static_cast<float>(x_size), static_cast<float>(y_size)});
    auto viewport_size = sf::FloatRect({0.f, 0.f}, {x_float, y_float});
    view.setSize({static_cast<float>(x_size), static_cast<float>(y_size)});
    view.setViewport(viewport_size);
}

static void calc_view(sf::RenderTexture &game_window, sf::View &view)
{
    unsigned int x_size = game_window.getSize().x;
    unsigned int y_size = game_window.getSize().y;

    float x_float = static_cast<float>(x_size / game_window.getSize().x);
    float y_float = static_cast<float>(y_size / game_window.getSize().y);
    auto window_size = sf::FloatRect({0.f, 0.f}, {static_cast<float>(x_size), static_cast<float>(y_size)});
    auto viewport_size = sf::FloatRect({0.f, 0.f}, {x_float, y_float});
    view.setSize({static_cast<float>(x_size), static_cast<float>(y_size)});
    view.setViewport(viewport_size);
}

} // namespace util