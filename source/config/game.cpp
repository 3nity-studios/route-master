#include "config/game.hpp"
#include "engine/state_machine.hpp"
#include "states/main_menu_state.hpp"
#include "states/map_viewer_state.hpp"
#include "states/store_state.hpp"

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"

// private
void Game::init_variables()
{
    this->_data->window = nullptr;
    this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    this->_data->player = Player(0, "Player1", 50000);

    this->_data->store = Store();
    this->_data->store.add_bus_to_inventory(Bus(1, "Bus1", 50, {}, 5), 100, 5);
    this->_data->store.add_bus_to_inventory(Bus(2, "Bus2", 60, {}, 10), 150, 3);
    this->_data->store.add_bus_to_inventory(Bus(3, "Bus3", 55, {}, 7), 120, 4);
    this->_data->store.add_bus_to_inventory(Bus(4, "Bus4", 45, {}, 3), 90, 6);
    this->_data->store.add_bus_to_inventory(Bus(5, "Bus5", 70, {}, 12), 200, 2);

    this->_data->store.add_employee_to_inventory(Employee(1, "John", "Doe", 25, 5, 0), 50, 1);
    this->_data->store.add_employee_to_inventory(Employee(2, "Jane", "Smith", 30, 7, 1), 60, 2);
    this->_data->store.add_employee_to_inventory(Employee(3, "Alice", "Johnson", 28, 6, 2), 55, 3);
    this->_data->store.add_employee_to_inventory(Employee(4, "Bob", "Brown", 35, 10, 3), 70, 1);
    this->_data->store.add_employee_to_inventory(Employee(5, "Charlie", "Davis", 40, 15, 4), 80, 2);


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
