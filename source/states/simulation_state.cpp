#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "engine/stops_manager.hpp"
#include "simulation/BusStop.hpp"
#include "utils/json_helper.hpp"
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <chrono>

float calc_distance(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

SimulationState::SimulationState(GameDataRef data)
    : _data(data), first_time(true), status("Picking up passengers"),
      bus_sim(Bus(1, "Bus 125", 32, std::list<Passenger>{}, 5)), driver_sim(Employee(0, "John", "Doe", 32, 12, 0)),
      bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)),
      bus_stops_texture(sf::Image(sf::Vector2u(100, 50), sf::Color::White)), bus(bus_texture)
{
    BusStop stop1(1, "Stop1", {10, 11, 10}, 2.0, 5.0, 3.0, 3.0, 2.0, 350.f, 5.f);
    BusStop stop2(2, "Stop2", {10, 11, 10}, 3.0, 10.0, 3.0, 3.0, 2.0, 500.f, 5.f);
    BusStop stop3(3, "Stop3", {10, 8, 9}, 15.0, 15.0, 3.0, 3.0, 2.0, 750.f, 5.f);
    BusStop stop4(4, "Stop4", {10, 11, 10}, 15.0, 15.0, 3.0, 3.0, 2.0, 350.f, 250.f);
    BusStop stop5(5, "Stop5", {11, 10, 11}, 15.0, 15.0, 3.0, 3.0, 2.0, 600.f, 250.f);
    BusStop stop6(6, "Stop6", {10, 11, 10}, 15.0, 15.0, 3.0, 3.0, 2.0, 600.f, 500.f);

    // conversion: bus_stop -> json
    // nlohmann::json j = stop1;
    // convert to JSON: copy each value into the JSON object
    nlohmann::json j;
    // std::string n = "assets/maps/stops1.json";
    std::ifstream f("assets/maps/stops1.json");
    j = nlohmann::json::parse(f);
    const auto stops_json_data = j["stops"];
    std::vector<BusStop> bus_stops_v;
    for (const auto &element : stops_json_data)
    {
        try
        {
            const int id_field{element.at("id")};
            const std::string name_field{element.at("name")};
            std::vector<int> aha_field;
            for (auto& num : element["avg_hourly_arrivals"])
                aha_field.push_back(num);
            const float aat_field{element.at("avg_arrival_time")};
            const float awt_field{element.at("avg_waiting_time")};
            const float sdw_field{element.at("sd_waiting_time")};
            const float abs_field{element.at("avg_bus_stop")};
            const float sdb_field{element.at("sd_bus_stop")};
            const float x_field{element.at("x")};
            const float y_field{element.at("y")};
            BusStop new_stop(id_field,name_field,aha_field,aat_field,awt_field,sdw_field,abs_field,sdb_field,x_field,y_field);
            bus_stops_v.push_back(new_stop);
        }
        catch (const std::out_of_range & exception)
        {
            // std::cerr("Error in JSON data from `{}`: unknown stop: '{}'.", json_path, std::string(element));
            throw exception;
        }
    }
    const auto lights_json_data = j["traffic_lights"];
    std::vector<TrafficLight> traffic_lights_v;
    for (const auto &element : lights_json_data)
    {
        try
        {
            const int id_field{element.at("id")};
            std::vector<std::pair<StreetConnectionIDs, bool>> connections_field;
            for (auto& con : element["connections"])
            {
                std::vector<int> street_ids_v;
                for (auto& id : con["street_ids"])
                {
                    street_ids_v.push_back(id);
                }
                const bool green_field{con.at("green_light")};
                std::pair<StreetConnectionIDs, bool> connection({street_ids_v.at(0), street_ids_v.at(1)}, green_field);
                connections_field.push_back(connection);
            }
            const int ttc_field{element.at("time_to_change")};
            const float x_field{element.at("x")};
            const float y_field{element.at("y")};
            TrafficLight new_light(id_field, connections_field, ttc_field, x_field, y_field);
            traffic_lights_v.push_back(new_light);
        }
        catch (const std::out_of_range & exception)
        {
            // std::cerr("Error in JSON data from `{}`: unknown stop: '{}'.", json_path, std::string(element));
            throw exception;
        }
    }
    const auto curves_json_data = j["curves"];
    std::vector<VisualElement> curves_v;
    for (const auto &element : curves_json_data)
    {
        try
        {
            const int id_field{element.at("id")};
            const float x_field{element.at("x")};
            const float y_field{element.at("y")};
            VisualElement new_curve(id_field, x_field, y_field);
            curves_v.push_back(new_curve);
        }
        catch (const std::out_of_range & exception)
        {
            // std::cerr("Error in JSON data from `{}`: unknown stop: '{}'.", json_path, std::string(element));
            throw exception;
        }
    }
    
    std::cout << j << std::endl;

    TrafficLight light1(7,
                        std::vector<std::pair<StreetConnectionIDs, bool>>{
                            std::make_pair<StreetConnectionIDs, bool>(std::make_pair<int, int>(4, 5), true)},
                        10, 450.f, 250.f);

    VisualElement curve1(8, 180.f, 200.f);
    VisualElement curve2(9, 800.f, 500.f);

    for (const auto &element : bus_stops_v)
    {
        std::cout << "adding bus_stop: " << element.name << std::endl;
        city.add_bus_stop(element);
    }
    for (const auto &element : traffic_lights_v)
    {
        std::cout << "adding traffic_light: " << element.id << std::endl;
        city.add_traffic_light(element);
    }
    for (const auto &element : curves_v)
    {
        std::cout << "adding curve: " << element.id << std::endl;
        city.add_curve(element);
    }

    city.initialize_bus_stops();

    Street street1(1, "Street1", calc_distance(stop1, stop2), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", calc_distance(stop2, stop3), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street3(3, "Street3", calc_distance(stop3, curve1), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street4(4, "Street4", calc_distance(curve1, stop4), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street5(5, "Street5", calc_distance(stop4, light1), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street6(6, "Street6", calc_distance(light1, stop5), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street7(7, "Street7", calc_distance(stop5, curve2), 50.0f, 2.0f, 0.1f, 0.05f);
    Street street8(8, "Street8", calc_distance(curve2, stop6), 50.0f, 2.0f, 0.1f, 0.05f);

    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    city.add_street(street3, 3, 8);
    city.add_street(street4, 8, 4);
    city.add_street(street5, 4, 7);
    city.add_street(street6, 7, 5);
    city.add_street(street7, 5, 9);
    city.add_street(street8, 9, 6);

    StreetArcList path;

    for (int i = 1; i < 9; i++)
    {
        for (auto street : city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path.append(street);
            }
        }
    }

    for (auto track : path)
    {
        elements_path.push_back(track->get_src_node()->get_info());
    }

    elements_path.push_back(path.get_last()->get_tgt_node()->get_info());

    simulation_info = city.run_simulation(bus_sim, driver_sim, 10, path);

    this->times.clear();

    set_simulation_parameters(simulation_info.times);

    actual_stop = 0;
}

SimulationState::SimulationState(GameDataRef data, std::list<std::pair<int, int>> _simulation_times,
                                 Bus _simulation_bus, Employee _simulation_driver)
    : _data(data), times(_simulation_times), first_time(true), bus_sim(_simulation_bus), driver_sim(_simulation_driver),
      status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus(bus_texture)
{
}

void calc_map_view(sf::RenderWindow &game_window, sf::View &map_view)
{
    uint x_size = game_window.getSize().x;
    uint y_size = game_window.getSize().y;

    float x_float = (float)x_size / static_cast<float>(game_window.getSize().x);
    float y_float = (float)y_size / static_cast<float>(game_window.getSize().y);
    auto window_size = sf::FloatRect({0.f, 0.f}, {static_cast<float>(x_size), static_cast<float>(y_size)});
    auto viewport_size = sf::FloatRect({0.f, 0.f}, {x_float, y_float});
    map_view.setSize({static_cast<float>(x_size), static_cast<float>(y_size)});
    map_view.setViewport(viewport_size);
}

void SimulationState::scroll_map_view(ScreenScrollDirection passed_scroll_direction, sf::View &map_view, sf::Clock game_clock, int scroll_speed)
{
    // std::cout << "game_clock.getElapsedTime().asSeconds()" << game_clock.getElapsedTime().asSeconds() << std::endl;
    switch (passed_scroll_direction)
    {
    case SCROLL_LEFT:
        map_view.move({static_cast<float>(-scroll_speed) * 0.0004f, 0});
        break;
    case SCROLL_RIGHT:
        map_view.move({static_cast<float>(scroll_speed) * 0.0004f, 0});
        break;
    case SCROLL_UP:
        map_view.move({0, static_cast<float>(-scroll_speed) * 0.0004f});
        break;
    case SCROLL_DOWN:
        map_view.move({0, static_cast<float>(scroll_speed) * 0.0004f});
        break;
    case NO_SCROLL:
    default:
        break;
    }
}

void zoom_view(float zoom_delta, sf::View &map_view)
{
    map_view.zoom(zoom_delta);
}

sf::Vector2i mouse_pos;

ScreenScrollDirection SimulationState::isScreenScrollRequired(sf::RenderWindow &game_window)
{
    mouse_pos = sf::Mouse::getPosition(game_window);
    // std::cout << "View center position x: " << this->_game_view.getCenter().x << std::endl;
    // std::cout << "View center position y: " << this->_game_view.getCenter().y << std::endl;
    // std::cout << "Map position 2x: " << this->_map.getBounds().width << std::endl;
    // std::cout << "Map position 2y: " << this->_map.getBounds().height << std::endl;
    if (this->_game_view.getCenter().x > game_window.getSize().x/2 && mouse_pos.x < 30 && mouse_pos.x >= 0)
    {
        return SCROLL_LEFT;
    }
    else if (this->_game_view.getCenter().x < this->_map.getBounds().width - game_window.getSize().x/2 && mouse_pos.x > game_window.getSize().x - 30 && mouse_pos.x <= game_window.getSize().x)
    {
        return SCROLL_RIGHT;
    }
    else if (this->_game_view.getCenter().y > game_window.getSize().y/2 && mouse_pos.y < 30 && mouse_pos.y >= 0)
    {
        return SCROLL_UP;
    }
    else if (this->_game_view.getCenter().y < this->_map.getBounds().height - game_window.getSize().y/2 && mouse_pos.y > game_window.getSize().y - 30 && mouse_pos.y <= game_window.getSize().y)
    {
        return SCROLL_DOWN;
    }

    return NO_SCROLL;
}

sf::View view;
float gameZoom;

void SimulationState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    if (!bus_texture.loadFromFile("assets/img/bus_top_view.png"))
    {
        throw GameException("Couldn't find file: assets/img/bus_top_view.png");
    }

    if (!bus_stops_texture.loadFromFile("assets/img/bus_stop_sprites.png"))
    {
        throw GameException("Couldn't find file: assets/img/bus_stop_sprites.png");
    }

    calc_map_view(*this->_data->window, _game_view);
    this->_data->window->setView(_game_view);
    init_bus_stops();
    init_bus();
    _map.load("assets/maps/demo.tmx");
}

void SimulationState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);

        if (const auto *mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            gameZoom += mouseWheel->delta;
            zoom_view(gameZoom, _game_view);
            this->_data->window->setView(_game_view);
        }

        if (event->is<sf::Event::Closed>())
        {
            this->_data->window->close();
            break;
        }

        if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>())
        {
            // When the enter key is pressed, switch to the next handler type
            if (keyPress->code == sf::Keyboard::Key::Escape)
            {
                this->_data->states.remove_state();
                break;
            }
        }
    }
}

// marks dt to not warn compiler
void SimulationState::update_state(float dt __attribute__((unused)))
{
    update_bus();
}

int calcScrollSpeed(sf::RenderWindow &gameWindow)
{
    mouse_pos = sf::Mouse::getPosition(gameWindow);

    if (mouse_pos.x < 30)
    {
        if (mouse_pos.x < 10)
        {
            return 60000;
        }
        return 30000;
    }
    else if (mouse_pos.x > gameWindow.getSize().x - 30)
    {
        if (mouse_pos.x > gameWindow.getSize().x - 10)
        {
            return 60000;
        }
        return 30000;
    }
    else if (mouse_pos.y < 30)
    {
        if (mouse_pos.y < 10)
        {
            return 60000;
        }
        return 30000;
    }
    else if (mouse_pos.y > gameWindow.getSize().y - 30)
    {
        if (mouse_pos.y > gameWindow.getSize().y - 10)
        {
            return 60000;
        }
        return 30000;
    }
    return 0;
}

void SimulationState::draw_state(float dt __attribute__((unused)))
{
    this->_data->window->clear(sf::Color::Black);

    MapLayer layerZero(_map, 0);
    MapLayer layerOne(_map, 1);
    MapLayer layerTwo(_map, 2);

    sf::Clock globalClock;
    scroll_map_view(isScreenScrollRequired(*this->_data->window), _game_view, globalClock, calcScrollSpeed(*this->_data->window));
    this->_data->window->setView(_game_view);
    this->_data->window->draw(layerZero);
    this->_data->window->draw(layerOne);

    sf::Font font("assets/fonts/joystix.ttf");
    sf::Text text(font);

    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    text.setString(status);

    text.setCharacterSize(12);

    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text.setPosition(sf::Vector2f(300.f, 500.f));

    this->_data->window->draw(text);

    for (auto bus_stop : bus_stops)
    {
        this->_data->window->draw(bus_stop);
    }

    for (auto bus_stop : passengers)
    {
        for (auto passenger : bus_stop)
        {
            this->_data->window->draw(passenger);
        }
    }

    this->_data->window->draw(bus);

    sf::Text text2(font);
    text2.setString("\nDriver: " + driver_sim.get_name() + " " + driver_sim.get_last_name() + "\n" +
                    "Bus: " + bus_sim.get_name());

    text2.setCharacterSize(12); // in pixels, not points!

    // set the color
    text2.setFillColor(sf::Color::White);

    // set the text style
    text2.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text2.setPosition(sf::Vector2f(300.f, 500.f));

    // inside the main loop, between window.clear() and window.display()
    this->_data->window->draw(text2);

    this->_data->window->draw(layerTwo);
    // Displays rendered objects
    this->_data->window->display();
}

void SimulationState::set_driver_sim(Employee _driver)
{
    driver_sim = _driver;
}

void SimulationState::set_bus_sim(Bus _bus)
{
    bus_sim = _bus;
}

float generateRandom(float lower, float upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(lower, upper);

    return std::round(dis(gen));
}

void SimulationState::init_bus_stops()
{
    for (auto visual_element : city.get_visual_elements())
    {
        auto stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (stop)
        {
            sf::Sprite bus_stop(bus_stops_texture);
            bus_stop.setTextureRect(sf::IntRect(sf::Vector2i(617, 200), sf::Vector2i(197, 104)));
            bus_stop.setPosition(
                sf::Vector2f(visual_element->get_info()->get_x(), visual_element->get_info()->get_y()));
            bus_stop.setScale(sf::Vector2<float>(0.5, 0.5));
            bus_stops.push_back(bus_stop);

            std::list<sf::CircleShape> passenger_list;

            for (int i = 0; i < stop->get_passenger_list().size(); i++)
            {
                std::uniform_real_distribution<float> dis(bus_stop.getPosition().x, bus_stop.getLocalBounds().size.x);

                sf::CircleShape passenger;

                passenger.setPosition(
                    sf::Vector2f(generateRandom(bus_stop.getPosition().x,
                                                bus_stop.getPosition().x + (0.5 * bus_stop.getLocalBounds().size.x)),
                                 bus_stop.getPosition().y + 60));

                passenger.setRadius(2.f);

                passenger_list.push_back(passenger);
            }

            passengers.push_back(passenger_list);
        }
    }
}

void SimulationState::update_bus_stops()
{
}

void SimulationState::init_bus()
{
    sf::IntRect bus_texture(sf::Vector2i(8, 5), sf::Vector2i(358, 657));

    bus.setTextureRect(bus_texture);

    sf::FloatRect bounds = bus.getLocalBounds();

    bus.setOrigin(bounds.getCenter());

    bus.rotate(sf::degrees(90));

    bus.setPosition(sf::Vector2f(385.f, 90.f));

    bus.setScale(sf::Vector2<float>(0.1, 0.1));
}

void SimulationState::update_bus()
{
    // sf::IntRect right_view(sf::Vector2i(0,0), sf::Vector2i(717,390));
    // sf::IntRect left_view(sf::Vector2i(0,1172), sf::Vector2i(711,385));

    if (times.empty())
    {
        status = "Route completed";
        return;
    }

    if (first_time)
    {
        simulation_clock.restart();
        first_time = false;
    }

    auto time = times.front();

    if ((time.first == 0 || time.first == 2 || time.first == 3) &&
        simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        if (time.first == 0)
        {
            auto passengers_leave = simulation_info.passengers.at(actual_stop).first;
            auto passengers_add = simulation_info.passengers.at(actual_stop).second;

            if (!passengers.at(actual_stop).empty())
            {
                for (int j = 0; j < passengers_leave; j++)
                {
                    if (passengers.at(actual_stop).size() > 0)
                    {
                        passengers.at(actual_stop).pop_front();
                    }
                }
            }
        }

        times.pop_front();
        time = times.front();
        simulation_clock.restart();
        auto stop1 = elements_path.front();
        elements_path.pop_front();
        auto stop2 = elements_path.front();

        sf::Angle rotation =
            sf::radians(M_PI - (atan((stop2->get_x() - stop1->get_x()) / (stop2->get_y() - stop1->get_y()))));

        bus.setRotation(rotation);

        bus_speed = sf::Vector2f((stop2->get_x() - stop1->get_x()) / (time.second * 60.f),
                                 (stop2->get_y() - stop1->get_y()) / (time.second * 60.f));
        status = "Travelling";
    }
    else if (time.first == 1 && simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        times.pop_front();
        time = times.front();

        if (times.empty())
        {
            status = "Dropping off passengers";
            actual_stop++;
        }
        else if (time.first == 0)
        {
            status = "Picking up and dropping off passengers";
            actual_stop++;
        }
        else if (time.first == 2)
        {
            status = "Waiting in Traffic Light";
        }

        simulation_clock.restart();
        bus_speed = sf::Vector2f(0.f, 0.f);
    }

    bus.move(bus_speed);
}

void SimulationState::set_simulation_parameters(std::list<std::pair<int, int>> _times)
{
    times = _times;
}

SimulationState::~SimulationState()
{
}
