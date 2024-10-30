#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "simulation/Bus.hpp"
#include "simulation/City.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <tmxlite/Map.hpp>

#include "engine/tmx_core.hpp"

class SimulationState : public Engine::State
{
  public:
    SimulationState(GameDataRef data);
    ~SimulationState();
    SimulationState(GameDataRef data, std::vector<SimulationInfo> _simulation_info);

    void init_state();
    void init_bus_stops();
    void init_bus();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void update_bus();
    void update_bus_stops();
    void draw_passengers();
    void draw_state(float dt __attribute__((unused)));

    void set_simulation_info(std::vector<SimulationInfo> _simulation_info);
    std::vector<SimulationInfo> get_simulation_info();

  private:
    GameDataRef _data;
    std::list<sf::Sprite> bus_stops;
    std::vector<SimulationInfo> simulation_info; 
    City city;
    sf::Clock simulation_clock;
    sf::Vector2f bus_speed;
    bool first_time;
    sf::String status;
    sf::Texture bus_texture;
    sf::Texture bus_stops_texture;
    sf::Texture person_texture; 
    int actual_stop;
    int current_time;
};
