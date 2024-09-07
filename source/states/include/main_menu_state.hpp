#pragma once

#include "config/include/game.hpp"
#include "config/include/global.hpp"
#include "states/include/state.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class MainMenuState : public Engine::State
{
  public:
    MainMenuState(GameDataRef data);
    //~MainMenuState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
};
