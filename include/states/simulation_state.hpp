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

enum class ActionState { NONE, PANNING };

enum ScreenScrollDirection
{
    NO_SCROLL = -1,
    SCROLL_LEFT,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN
};

class SimulationState : public Engine::State
{
  public:
    SimulationState(GameDataRef data);
    ~SimulationState();
    SimulationState(GameDataRef data, std::list<std::pair<int, int>> _simulation_times, Bus _simulation_bus, Employee _simulation_driver);

    void init_state();
    void init_bus_stops();
    void init_bus();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void update_bus();
    void update_bus_stops();
    void draw_state(float dt __attribute__((unused)));

    void set_driver_sim(Employee _driver);
    void set_bus_sim(Bus _bus); 
    void set_simulation_parameters(std::list<std::pair<int, int>> _times);
    ScreenScrollDirection is_screen_scroll_required(sf::RenderWindow &gameWindow);
    void scroll_map_view(ScreenScrollDirection passed_ScrollDirection, sf::View &mapView, sf::Clock gameClock, int scrollSpeed);
  private:
    GameDataRef _data;
    
    std::list<sf::Sprite> bus_stops;
    std::list<std::pair<int, int>> times;
    std::list<std::shared_ptr<VisualElement>> elements_path;
    std::list<int> bus_stops_passengers_amount;
    std::vector<std::list<sf::CircleShape>> passengers;
    SimulationInfo simulation_info; 
    sf::Sprite bus;
    sf::Clock simulation_clock;
    Employee driver_sim; 
    Bus bus_sim; 
    sf::Vector2f bus_speed;
    bool first_time;
    sf::String status;
    City city;
    sf::Texture bus_texture;
    sf::Texture bus_stops_texture;
    int actual_stop;

    sf::View _game_view;
    sf::View _gui_view;
    tmx::Map _map;
    sf::Vector2i _panning_anchor;
    float _zoom_level;
    ActionState _action_state;
};
