#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "simulation/City.hpp"
#include "states/state.hpp"
#include <SFML/Graphics.hpp>
#include <Designar/graphalgorithms.hpp>

#include <tmxlite/Map.hpp>

#include "engine/tmx_core.hpp"

class RouteSelect : public Engine::State
{
  public:
    RouteSelect(GameDataRef data);
    ~RouteSelect();

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
    std::list<std::pair<sf::Sprite, std::shared_ptr<VisualElement>>> visual_elements;
    sf::Texture map_icons_texture;
    std::vector<std::shared_ptr<VisualElement>> new_path;
    bool sprite_pressed;

    void init_visual_elements(); 
    void draw_lines();
    bool add_to_path(std::shared_ptr<VisualElement> visual_element);
};
