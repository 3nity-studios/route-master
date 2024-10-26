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
    SimulationState(GameDataRef data, SimulationInfo);

    void init_state();
    void init_bus_stops();
    void init_bus();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void update_bus();
    void update_bus_stops();
    void draw_state(float dt __attribute__((unused)));

    void set_simulation_info(SimulationInfo _simulation_info);
    SimulationInfo get_simulation_info();

  private:
    GameDataRef _data;
    std::list<sf::Sprite> bus_stops;
    std::list<std::shared_ptr<VisualElement>> elements_path;
    std::list<int> bus_stops_passengers_amount;
    std::vector<std::list<sf::CircleShape>> passengers;
    SimulationInfo simulation_info; 
    sf::Sprite bus;
    sf::Clock simulation_clock;
    sf::Vector2f bus_speed;
    bool first_time;
    sf::String status;
    sf::Texture bus_texture;
    sf::Texture bus_stops_texture;
    int actual_stop;
};
