#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class MainMenuState : public Engine::State
{
  public:
    MainMenuState(GameDataRef data);
    //~MainMenuState() = default;
    MainMenuState(GameDataRef data, std::list<std::pair<int, int>> _simulation_times, int _simulation_stops_number);

    void init_state();
    void init_bus_stops();
    void init_bus();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void update_bus();
    void draw_state(float dt __attribute__((unused)));

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
};
