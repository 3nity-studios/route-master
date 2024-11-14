#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "simulation/City.hpp"
#include "states/state.hpp"
#include <SFML/Graphics.hpp>
#include <Designar/graphalgorithms.hpp>

#include <tmxlite/Map.hpp>

#include "engine/tmx_core.hpp"
#include <memory>
#include "utils/view_dragger.hpp"

class RouteEditState : public Engine::State
{
  public:
    RouteEditState(GameDataRef data, Route &_route);
    ~RouteEditState();

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
    std::list<std::tuple<sf::Sprite, std::shared_ptr<VisualElement>, bool>> visual_elements;
    sf::Texture map_icons_texture;
    Route &route;
    bool sprite_pressed;
    bool create_new_path; 
    tgui::CanvasSFML::Ptr canvas;
    Route route_copy;
    std::vector<std::shared_ptr<VisualElement>> redo_stack;
    std::optional<util::ViewDragger> _view_dragger;
    tmx::Map _map;

    void init_visual_elements(); 
    void draw_lines();
    bool add_to_path(std::shared_ptr<VisualElement> visual_element);
};
