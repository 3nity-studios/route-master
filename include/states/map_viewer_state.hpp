#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <tmxlite/Map.hpp>

#include "engine/tmx_core.hpp"

// class ViewDragger
// {
//   public:
//     /// set render target with view and initialize dragging to false
//     ViewDragger(sf::RenderTarget &target) : target{target}, dragging{}
//     {
//         std::cout << "VIEWDRAGGER CREATED!";
//     }

//     void setTarget(sf::RenderTarget &target2)
//     {
//         this->target = std::move(target2);
//     }
//     /// handle dragging related events
//     void handleEvent(const sf::Event event)
//     {

//     }

//   private:
//     /// the render target with the view we want to change
//     sf::RenderTarget &target;
//     /// the last known mouse position
//     sf::Vector2i previous_mouse_position;
//     /// whether we are dragging or not
//     bool dragging;
//     GameDataRef _data;
// };
class MapViewerState : public Engine::State
{
  public:
    MapViewerState(GameDataRef data);
    //~MapViewerState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
    // ViewDragger view_dragger;
};
