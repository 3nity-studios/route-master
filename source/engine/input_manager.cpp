#include "engine/input_manager.hpp"

namespace Engine
{

bool is_sprite_clicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window)
{
    if (sf::Mouse::isButtonPressed(button))
    {
        sf::IntRect playButtonRect(
            {static_cast<int>(object.getPosition().x), static_cast<int>(object.getPosition().y)},
            {static_cast<int>(object.getGlobalBounds().size.x), static_cast<int>(object.getGlobalBounds().size.y)});

        if (playButtonRect.contains(sf::Mouse::getPosition(window)))
        {
            return true;
        }
    }

    return false;
}

sf::Vector2i InputManager::get_mouse_position(sf::RenderWindow &window)
{
    return sf::Mouse::getPosition(window);
}
} // namespace Engine
