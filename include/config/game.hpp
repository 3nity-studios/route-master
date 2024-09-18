#include "config/global.hpp"
#include "engine/asset_manager.hpp"
#include "engine/input_manager.hpp"
#include "engine/state_machine.hpp"
#include "platform/platform.hpp"

#pragma once

struct GameData
{
    sf::RenderWindow *window;
    Engine::StateMachine states;
    Engine::InputManager inputs;
    Engine::AssetManager assets;
};

typedef std::shared_ptr<GameData> GameDataRef;
class Game
{

  private:
    // For platform specific methods
    util::Platform _platform;

    // Game Variables
    sf::VideoMode _video_mode;

    // Private Functions
    void init_variables();
    void init_window();

    // Game Data ref
    GameDataRef _data = std::make_shared<GameData>();

    // Time data
    const float dt = 1.0f / 60.0f;
    sf::Clock _clock;

  public:
    // Constructors and Destructors
    Game();
    virtual ~Game();

    // Accessors
    bool is_running() const;
    void run();
};

// custom exception to use for debugging
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
