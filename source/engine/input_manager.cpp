#include "engine/input_manager.hpp"

namespace Engine
{

bool InputManager::is_sprite_clicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window)
{
    if (sf::Mouse::isButtonPressed(button))
    {
        sf::IntRect object_rect(static_cast<sf::Vector2i>(object.getPosition()),
                                static_cast<sf::Vector2i>(object.getGlobalBounds().size));

        return object_rect.contains(sf::Mouse::getPosition(window));
    }

    return false;
}

bool InputManager::is_sprite_clicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window,
                                     sf::RenderTarget &target)
{
    sf::View view = target.getView();
    sf::Vector2f mouse_world_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

    if (sf::Mouse::isButtonPressed(button))
    {
        sf::IntRect object_rect(static_cast<sf::Vector2i>(object.getPosition()),
                                static_cast<sf::Vector2i>(object.getGlobalBounds().size));

        return object_rect.contains(static_cast<sf::Vector2i>(mouse_world_pos));
    }

    return false;
}

sf::Vector2i InputManager::get_mouse_position(sf::RenderWindow &window)
{
    return sf::Mouse::getPosition(window);
}
} // namespace Engine
