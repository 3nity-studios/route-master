#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/simulation_state.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class BusSelectState : public Engine::State
{
  public:
    BusSelectState(GameDataRef data);
    //~StoreState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));
    
  private:
    GameDataRef _data;
    SimulationInfo new_simulation_info;
    Bus bus; 
    Employee employee; 
    int selected_path;

    tgui::Panel::Ptr create_selection_panel();
};