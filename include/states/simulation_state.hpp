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
#include <memory>
#include "utils/view_dragger.hpp"

#include <SFML/Audio/Music.hpp>
class SimulationState : public Engine::State
{
  public:
    SimulationState(GameDataRef data);
    ~SimulationState();
    SimulationState(GameDataRef data, std::vector<SimulationInfo> _simulation_info);

    void init_state();
    void init_bus();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void update_bus();
    void update_bus_stops();
    void draw_passengers(sf::Font font);
    void draw_state(float dt __attribute__((unused)));
    void resume_state() override;
    void pause_state() override;
    void add_simulation_info(SimulationInfo _simulation_info);
    void manage_collisions(SimulationInfo &info, int i);
    void check_is_visible(SimulationInfo &info_to_check);

    void set_simulation_info(std::vector<SimulationInfo> _simulation_info);
    std::vector<SimulationInfo> get_simulation_info();

  private:
    GameDataRef _data;
    std::list<sf::Sprite> bus_stops;
    sf::Clock simulation_clock;
    sf::Vector2f bus_speed;
    bool first_time;
    sf::String status;
    sf::Texture bus_texture;
    sf::Texture bus_stops_texture;
    sf::Texture person_texture;
    int actual_stop;
    int current_time;
    tgui::Gui gui; 
    tmx::Map _map;
    std::unique_ptr<util::ViewDragger> _view_dragger;
    sf::View _view;
    sf::Music music;
};
