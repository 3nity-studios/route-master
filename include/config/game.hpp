#pragma once

#include "config/global.hpp"
#include "engine/asset_manager.hpp"
#include "engine/input_manager.hpp"
#include "engine/state_machine.hpp"
#include "platform/platform.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "player/Player.hpp"
#include "player/Store.hpp"
#include "player/AchievementManager.hpp"
#include "simulation/City.hpp"
#include "simulation/Route.hpp"

struct GameData
{
    sf::RenderWindow *window;
    Engine::StateMachine states;
    Engine::InputManager inputs;
    Engine::AssetManager assets;
    tgui::Gui gui;

    Player player;
    Store store;
    std::vector<Route> routes;
    City city;
    std::vector<SimulationInfo> simulation_info;
    AchievementManager achievement_manager;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game
{
  private:
    util::Platform _platform;
    sf::VideoMode _video_mode;
    GameDataRef _data = std::make_shared<GameData>();
    const float dt = 1.0f / 60.0f;
    sf::Clock _clock;

    void init_variables();
    void init_window();

  public:
    Game();
    virtual ~Game();

    bool is_running() const;
    void run();
};

struct GameException : public std::exception
{
  protected:
    const char *error_msg;

  public:
    GameException(const char *str)
    {
        error_msg = str;
    }

    const char *what() const throw()
    {
        return error_msg;
    }
};
