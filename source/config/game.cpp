#include "config/game.hpp"
#include "engine/state_machine.hpp"
#include "states/main_menu_state.hpp"
#include "states/map_viewer_state.hpp"
#include "states/store_state.hpp"

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"

float calc_distance(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

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

    this->_data->store.add_employee_to_inventory(Employee(1, "John", "Doe", 25, 50, 5, 0), 50, 1);
    this->_data->store.add_employee_to_inventory(Employee(2, "Jane", "Smith", 30, 60, 7, 1), 60, 2);
    this->_data->store.add_employee_to_inventory(Employee(3, "Alice", "Johnson", 28, 55, 6, 2), 55, 3);
    this->_data->store.add_employee_to_inventory(Employee(4, "Bob", "Brown", 35, 70, 10, 3), 70, 1);
    this->_data->store.add_employee_to_inventory(Employee(5, "Charlie", "Davis", 40, 80, 15, 4), 80, 2);

    BusStop stop1(1, "Stop1", {2, 3, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 750.f, 5.f);
    BusStop stop2(2, "Stop2", {2, 3, 4}, 3.0, 10.0, 3.0, 3.0, 2.0, 500.f, 5.f);
    BusStop stop3(3, "Stop3", {2, 3, 3}, 3.0, 5.0, 3.0, 3.0, 2.0, 350.f, 5.f);
    BusStop stop4(4, "Stop4", {2, 4, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 350.f, 250.f);
    BusStop stop5(5, "Stop5", {2, 5, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 600.f, 250.f);
    BusStop stop6(6, "Stop6", {2, 6, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 600.f, 500.f);

    TrafficLight light1(7, std::vector<std::pair<StreetConnectionIDs, bool>>{std::make_pair<StreetConnectionIDs, bool>(std::make_pair<int, int>(6,7), true)}, 10, 450.f, 250.f);

    VisualElement curve1(8, 220.f, 50.f);
    VisualElement curve2(9, 220.f, 250.f);
    VisualElement curve3(10, 745.f, 275.f);
    VisualElement curve4(11, 745.f, 500.f);

    this->_data->city.add_bus_stop(stop1);
    this->_data->city.add_bus_stop(stop2);
    this->_data->city.add_bus_stop(stop3);
    this->_data->city.add_bus_stop(stop4);
    this->_data->city.add_traffic_light(light1);
    this->_data->city.add_bus_stop(stop5);
    this->_data->city.add_bus_stop(stop6);
    this->_data->city.add_curve(curve1);
    this->_data->city.add_curve(curve2);
    this->_data->city.add_curve(curve3);
    this->_data->city.add_curve(curve4);

    this->_data->city.initialize_bus_stops();

    Street street1(1, "Street1", calc_distance(stop1, stop2), 200.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", calc_distance(stop2, stop3), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street3(3, "Street3", calc_distance(stop3, curve1), 100.0f, 2.0f, 0.1f, 0.05f);
    Street street4(4, "Street4", calc_distance(curve1, curve2), 25.0f, 2.0f, 0.1f, 0.05f);
    Street street5(5, "Street5", calc_distance(curve2, stop4), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street6(6, "Street6", calc_distance(stop4, light1), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street7(7, "Street7", calc_distance(light1, stop5), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street8(8, "Street8", calc_distance(stop5, curve3), 100.0f, 2.0f, 0.1f, 0.05f);
    Street street9(9, "Street9", calc_distance(curve3, curve4), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street10(10, "Street10", calc_distance(curve4, stop6), 50.0f, 2.0f, 0.1f, 0.05f);

    this->_data->city.add_street(street1, 1, 2);
    this->_data->city.add_street(street2, 2, 3);
    this->_data->city.add_street(street3, 3, 8);
    this->_data->city.add_street(street4, 8, 9);
    this->_data->city.add_street(street5, 9, 4);
    this->_data->city.add_street(street6, 4, 7);
    this->_data->city.add_street(street7, 7, 5);
    this->_data->city.add_street(street8, 5, 10);
    this->_data->city.add_street(street9, 10, 11);
    this->_data->city.add_street(street10, 11, 6);

    StreetArcList path;

    for (int i = 1; i < 6; i++)
    {
        for (auto street : this->_data->city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path.append(street);
            }
        }
    }

    StreetArcList path2;

    for (int i = 6; i < 11; i++)
    {
        for (auto street : this->_data->city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path2.append(street);
            }
        }
    }

    Bus bus_sim(1, "Bus 1", 10, {}, 5);
    Employee driver_sim(1, "John", "Doe", 25, 50, 5, 0);

    Bus bus_sim2(2, "Bus 2", 15, {}, 5);
    Employee driver_sim2(2, "Rodrigo", "Hernandez", 23, 30, 12, 0);

    SimulationInfo simulation(bus_sim, driver_sim, path);
    SimulationInfo simulation2(bus_sim2, driver_sim2, path2);

    this->_data->paths.push_back(simulation.elements_path);
    this->_data->paths.push_back(simulation2.elements_path);

    this->_data->city.update();
    this->_data->city.update_passengers();
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
