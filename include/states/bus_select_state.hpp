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
    void update_buttons(); 
    void draw_table(sf::Font font); 
    
  private:
    GameDataRef _data;
    SimulationInfo new_simulation_info;
    tgui::Gui buttons_gui; 
    std::string advertisement; 
    int selected_path;
};