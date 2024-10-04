#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "simulation/Bus.hpp"
#include "simulation/City.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class SimulationState : public Engine::State
{
  public:
    SimulationState(GameDataRef data);
    //~SimulationState() = default;
    SimulationState(GameDataRef data, std::list<std::pair<int, int>> _simulation_times, int _simulation_stops_number, Bus _simulation_bus, Employee _simulation_driver);

    void init_state();
    void init_bus_stops();
    void init_bus();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void update_bus();
    void draw_state(float dt __attribute__((unused)));

    void set_driver_sim(Employee _driver);
    void set_bus_sim(Bus _bus); 
    void set_simulation_parameters(std::list<std::pair<int, int>> _times, int _stops_number);

  private:
    GameDataRef _data;
    std::list<sf::RectangleShape> bus_stops;
    std::list<std::pair<int, int>> times;
    sf::RectangleShape bus;
    sf::Clock simulation_clock;
    sf::Vector2f bus_speed;
    bool first_time;
    sf::String status;
    int stops_number;
    City city;
    Employee driver_sim; 
    Bus bus_sim; 
};
