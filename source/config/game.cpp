#include "config/game.hpp"
#include "engine/state_machine.hpp"
#include "states/main_menu_state.hpp"
#include "states/map_viewer_state.hpp"

// private
void Game::init_variables()
{
    this->_data->window = nullptr;
    this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
}
void Game::init_window()
{
    // creating our window object
    this->_data->window = new sf::RenderWindow();

    // in Windows at least, this must be called before creating the window
    float screen_scaling_factor = _platform.get_screen_scaling_factor(this->_data->window->getNativeHandle());

    // Use the screenScalingFactor to create video mode and set screen size - 720p by default
    this->_video_mode.size = {static_cast<uint>(SCREEN_WIDTH * screen_scaling_factor),
                              static_cast<uint>(SCREEN_HEIGHT * screen_scaling_factor)};

    // creating our window view using the video mode and disabling resizablilty
    this->_data->window->create(this->_video_mode, "Route Master", sf::Style::Titlebar | sf::Style::Close);
    _platform.set_icon(this->_data->window->getNativeHandle());

    // sets FPS vsync
    this->_data->window->setFramerateLimit(60);
}

// Constructors

Game::Game()
{
    this->init_variables();
    this->init_window();

    // running the new game
    this->run();
}

Game::~Game()
{
    delete this->_data->window;
}

// Accessors
bool Game::is_running() const
{
    return this->_data->window->isOpen();
}

void Game::run()
{
    // initializing a new Game

    // time initialization
    float new_time, frame_time, interpolation;
    float current_time = this->_clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    // Game Loop
    while (this->is_running())
    {

        this->_data->states.process_states();

        new_time = this->_clock.getElapsedTime().asSeconds();
        frame_time = new_time - current_time;

        if (frame_time > 0.25f)
        {
            frame_time = 0.25f;
        }

        current_time = new_time;
        accumulator += frame_time;

        while (accumulator >= dt)
        {
            this->_data->states.get_active_state()->update_inputs();
            this->_data->states.get_active_state()->update_state(dt);

            accumulator -= dt;
        }

        interpolation = accumulator / dt;
        this->_data->states.get_active_state()->draw_state(interpolation);
    }
}
