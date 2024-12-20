#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/simulation_state.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class RouteListState : public Engine::State
{
  public:
    RouteListState(GameDataRef data);
    //~RouteListState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
    tgui::ScrollablePanel::Ptr create_route_list();
};
