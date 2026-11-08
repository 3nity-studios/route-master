#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/simulation_state.hpp"
#include "states/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Music.hpp>

class MainMenuState : public Engine::State
{
  public:
    MainMenuState(GameDataRef data);
    //~MainMenuState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt);
    void draw_state(float dt);

  private:
    GameDataRef _data;
    sf::Music music;
};
