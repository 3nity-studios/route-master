#include "config/game.hpp"
#include "engine/state_machine.hpp"
#include "states/main_menu_state.hpp"
#include "states/map_viewer_state.hpp"
#include "states/store_state.hpp"

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include "utils/street_factory.hpp"

float calc_distance(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

// private
void Game::init_variables()
{
    this->_data->window = nullptr;
    this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);

    std::ifstream player_file("data/player.json");
    if (player_file.is_open())
    {
        nlohmann::json player_json;
        player_file >> player_json;
        this->_data->player = Player(player_json);
        player_file.close();
    }
    else
    {
        this->_data->player = Player(0, "Player1", 80000);
        this->_data->player.save();
    }

    // Load store data from JSON if it exists
    std::ifstream store_file("data/store.json");
    if (store_file.is_open())
    {
        nlohmann::json store_json;
        store_file >> store_json;
        this->_data->store = Store(store_json);
        store_file.close();
    }
    else
    {
        this->_data->store = Store();
        this->_data->store.add_bus_to_inventory(Bus(0, "Encava", 50, {}, 5, 100, 500, 80, 150, 60, 150, 80, 150), 35000, 10);
        this->_data->store.add_bus_to_inventory(Bus(1, "Gillig Low-Floor", 35, {}, 10, 150, 650, 120, 200, 90, 200, 100, 200), 40000, 5);
        this->_data->store.add_bus_to_inventory(Bus(2, "Gillig ADA", 55, {}, 7, 120, 800, 100, 300, 80, 300, 150, 300), 58800, 3);

        this->_data->store.add_employee_to_inventory(Employee(0, "Jane", "Smith", 30, 20, 7, 1), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(1, "Alice", "Johnson", 28, 20, 6, 2), 450, 1);
        this->_data->store.add_employee_to_inventory(Employee(2, "Bob", "Brown", 35, 70, 20, 3), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(3, "Charlie", "Davis", 40, 20, 15, 4), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(4, "Abdul", "Qureshi", 32, 20, 8, 5), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(5, "Erick", "Moukoko", 29, 20, 7, 6), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(6, "Hans", "Schlafzimmer", 45, 20, 12, 7), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(7, "Katarina", "Marinkovic", 27, 20, 6, 8), 450, 1);
        this->_data->store.add_employee_to_inventory(Employee(8, "Nanako", "Miyamoto", 30, 20, 7, 9), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(9, "Juliana", "Contreras", 33, 20, 9, 10), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(10, "Sofía", "Pérez", 26, 50, 5, 20), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(11, "Pedro", "López", 28, 55, 6, 20), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(12, "Alejandro", "Muñoz", 31, 47, 8, 13), 500, 1);

        this->_data->store.save();
    }

    std::ifstream city_file("data/city.json");
    if (city_file.is_open())
    {
        nlohmann::json city_json;
        city_file >> city_json;
        this->_data->city = City(city_json);
        city_file.close();
    }
    else
    {
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

        Street street1 = util::StreetFactory(_data, 1, "Street1", 1, 2, 200.0f, 2.0f, 0.1f, 0.05f);
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

        this->_data->city.update();
        this->_data->city.update_passengers(); 
    }

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

    Bus bus_sim(1, "Bus 1", 10, {}, 5, 100, 5, 80, 4, 60, 3, 40, 2);
    Employee driver_sim(1, "John", "Doe", 25, 50, 5, 0);

    Bus bus_sim2(2, "Bus 2", 15, {}, 5, 150, 7, 120, 6, 90, 5, 70, 3);
    Employee driver_sim2(2, "Rodrigo", "Hernandez", 23, 30, 12, 0);

    SimulationInfo simulation(&bus_sim, &driver_sim, path);
    SimulationInfo simulation2(&bus_sim2, &driver_sim2, path2);

    this->_data->routes.push_back(Route("Route 1", simulation.elements_path));
    this->_data->routes.push_back(Route("Route 2", simulation2.elements_path));
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
