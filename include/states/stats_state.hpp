#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "simulation/City.hpp"

class StatsState : public Engine::State
{
  public:
    StatsState(GameDataRef data);
    //~StatsState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));
    void update_items_to_show(tgui::String filter);

  private:
    GameDataRef _data;
    std::vector<SimulationInfo> infos;
    City city;
};
