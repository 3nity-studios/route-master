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
    void update_state(float dt);
    void draw_state(float dt);

  private:
    GameDataRef _data;
    int current_info;
};
