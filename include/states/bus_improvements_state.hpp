#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class BusImprovementsState : public Engine::State
{
  public:
    BusImprovementsState(GameDataRef data, const int _bus_id);
    //~BusImprovementsState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt);
    void draw_state(float dt);

  private:
    GameDataRef _data;
    int bus_id;
};
