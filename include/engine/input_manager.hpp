#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Engine
{
class InputManager
{
  public:
    InputManager()
    {
    }
    ~InputManager()
    {
    }

    bool is_sprite_clicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window);

    bool is_sprite_clicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window,
                           sf::RenderTarget &target);

    sf::Vector2i get_mouse_position(sf::RenderWindow &window);
};
} // namespace Engine
