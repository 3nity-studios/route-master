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

#include <SFML/Graphics/View.hpp>
#include <tmxlite/Map.hpp>

#include "engine/tmx_core.hpp"
#include <memory>
#include "utils/view_dragger.hpp"

class SettingsState : public Engine::State
{
  public:
    SettingsState(GameDataRef data);
    // ~SettingsState();

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));
    
    void save_settings();

  private:
    GameDataRef _data;
    sf::View _view;
};
