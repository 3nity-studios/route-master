#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace util
{
class ViewDragger
{
  public:
    /// set render target with view and initialize dragging to false
    ViewDragger(sf::RenderTarget &target) : _target{target}, _dragging{false}
    {
    }
    /// handle dragging related events
    void handleEvent(const sf::Event event)
    {
        if (const auto *keyPress = event.getIf<sf::Event::MouseButtonPressed>())
        {
            // if mouse button is pressed start dragging
            if (keyPress->button == sf::Mouse::Button::Right)
            {
                _dragging = true;
                return;
            }
        }
        if (const auto *keyPress = event.getIf<sf::Event::MouseButtonReleased>())
        {
            // if mouse button is released stop draggin
            if (keyPress->button == sf::Mouse::Button::Right)
            {
                _dragging = false;
                return;
            }
        }
        // if dragging mouse
        if (const auto *keyPress = event.getIf<sf::Event::MouseMoved>())
        {
            // get mouse position
            const sf::Vector2i mouse_position{keyPress->position.x, keyPress->position.y};
            // if dragging, move view
            if (_dragging)
            {
                // calculate how far mouse has moved in view
                const auto delta =
                    _target.mapPixelToCoords(mouse_position) - _target.mapPixelToCoords(_previous_mouse_position);
                // apply negatively to view
                auto view = _target.getView();
                view.move(-delta);
                _target.setView(view);
            }
            // update previous mouse position
            _previous_mouse_position = mouse_position;
            return;
        }
    }

  private:
    /// the render target with the view we want to change
    sf::RenderTarget &_target;
    /// the last known mouse position
    sf::Vector2i _previous_mouse_position;
    /// whether we are dragging or not
    bool _dragging;
};
} // namespace util
