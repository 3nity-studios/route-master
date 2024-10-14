#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/simulation_state.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"

class StoreState : public Engine::State
{
  public:
    StoreState(GameDataRef data);
    //~StoreState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
    Designar::SLList<Bus> buses;
    Designar::SLList<Employee> employees;
};
