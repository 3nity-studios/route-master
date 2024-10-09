#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include <cmath>

float calc_distance(BusStop bus_stop_1, BusStop bus_stop_2)
{
    return sqrt(pow(bus_stop_1.get_x() - bus_stop_2.get_x(), 2) + pow(bus_stop_1.get_y() - bus_stop_2.get_y(), 2));
}

SimulationState::SimulationState(GameDataRef data) : _data(data), first_time(true), status("Picking up passengers"), bus_sim(Bus(1, "Bus 125", 32, std::list<Passenger>{}, 5)), driver_sim(Employee ("John", "Doe", 32, 12, 0)), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus_stops_texture(sf::Image(sf::Vector2u(100, 50), sf::Color::White)), bus(bus_texture)
{
    BusStop stop1(1, "Stop1", {5}, 5.0, 5.0, 3.0, 3.0, 2.0, 5.f, 5.f);
    BusStop stop2(2, "Stop2", {5}, 10.0, 10.0, 3.0, 3.0, 2.0, 150.f, 5.f);
    BusStop stop3(3, "Stop3", {5}, 15.0, 15.0, 3.0, 3.0, 2.0, 400.f, 5.f);
    BusStop stop4(4, "Stop4", {5}, 15.0, 15.0, 3.0, 3.0, 2.0, 5.f, 250.f);
    BusStop stop5(5, "Stop5", {5}, 15.0, 15.0, 3.0, 3.0, 2.0, 200.f, 250.f);
    BusStop stop6(6, "Stop6", {5}, 15.0, 15.0, 3.0, 3.0, 2.0, 200.f, 500.f);

    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);
    city.add_bus_stop(stop4);
    city.add_bus_stop(stop5);
    city.add_bus_stop(stop6);

    city.initialize_bus_stops();

    Street street1(1, "Street1", calc_distance(stop1, stop2), 10.0f, 2.0f, 0.1f);
    Street street2(2, "Street2", calc_distance(stop2, stop3), 10.0f, 2.0f, 0.1f);
    Street street3(3, "Street3", calc_distance(stop3, stop4), 10.0f, 2.0f, 0.1f);
    Street street4(4, "Street4", calc_distance(stop4, stop5), 10.0f, 2.0f, 0.1f);
    Street street5(5, "Street5", calc_distance(stop5, stop6), 10.0f, 2.0f, 0.1f);

    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    city.add_street(street3, 3, 4);
    city.add_street(street4, 4, 5);
    city.add_street(street5, 5, 6);

    StreetArcList path;

    for (int i = 1; i < 6; i++)
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
        path_bus_stops.push_back(track->get_src_node()->get_info());
    }

    path_bus_stops.push_back(path.get_last()->get_tgt_node()->get_info());

    auto time = city.run_simulation(bus_sim, driver_sim, 10, path);

    this->times.clear();

    set_simulation_parameters(time);

    if (!bus_texture.loadFromFile("assets/img/bus_sprites.png"))
    {
        throw GameException("Couldn't find file: assets/img/bus_sprites.png");
    }

    if (!bus_stops_texture.loadFromFile("assets/img/bus_stop_sprites.png"))
    {
        throw GameException("Couldn't find file: assets/img/bus_stop_sprites.png");
    }
}

SimulationState::SimulationState(GameDataRef data, std::list<std::pair<int, int>> _simulation_times,
                                 Bus _simulation_bus, Employee _simulation_driver)
    : _data(data), times(_simulation_times), first_time(true), bus_sim (_simulation_bus), driver_sim(_simulation_driver),
      status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus(bus_texture)
{
}

void SimulationState::init_state()
{
    init_bus_stops();
    init_bus();
}

void SimulationState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);

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

// marks dt to not warn compiler
void SimulationState::draw_state(float dt __attribute__((unused)))
{
    // SAMPLE RENDER CODE:

    // background color
    this->_data->window->clear(sf::Color::Black);

    // write text
    sf::Font font("assets/fonts/joystix.ttf");
    sf::Text text(font);

    // throws error if can't load font
    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        // error...
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    // set the string to display
    text.setString(status);

    // set the character size
    text.setCharacterSize(12); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text.setPosition(sf::Vector2f(400.f, 500.f));

    // inside the main loop, between window.clear() and window.display()
    this->_data->window->draw(text);

    // END SAMPLE RENDER CODE

    for (auto bus_stop : bus_stops)
    {
        this->_data->window->draw(bus_stop);
    }

    this->_data->window->draw(bus);

    sf::Text text2(font);
    // set the string to display
    text2.setString("\nDriver: " + driver_sim.get_name() + " " + driver_sim.get_last_name() + "\n" + "Bus: " + bus_sim.get_name());

    // set the character size
    text2.setCharacterSize(12); // in pixels, not points!

    // set the color
    text2.setFillColor(sf::Color::White);

    // set the text style
    text2.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text2.setPosition(sf::Vector2f(400.f, 500.f));

    // inside the main loop, between window.clear() and window.display()
    this->_data->window->draw(text2);

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

void SimulationState::init_bus_stops()
{
    for (auto stop : city.get_bus_stops())
    {
        sf::Sprite bus_stop(bus_stops_texture);

        bus_stop.setTextureRect(sf::IntRect(sf::Vector2i(617,200), sf::Vector2i(197,104)));

        bus_stop.setPosition(sf::Vector2f(stop->get_info().get_x(), stop->get_info().get_y()));

        bus_stop.setScale(sf::Vector2<float>(0.5, 0.5));

        bus_stops.push_back(bus_stop);
    }
}

void SimulationState::init_bus()
{
    sf::IntRect bus_texture(sf::Vector2i(0,0), sf::Vector2i(717,390));

    bus.setTextureRect(bus_texture);

    bus.setPosition(sf::Vector2f(5.f, 20.f));

    bus.setScale(sf::Vector2<float>(0.1, 0.1));
}

void SimulationState::update_bus()
{
    sf::IntRect right_view(sf::Vector2i(0,0), sf::Vector2i(717,390));
    sf::IntRect left_view(sf::Vector2i(0,1172), sf::Vector2i(711,385));

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

    if (time.first == 0 && simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        times.pop_front();
        time = times.front();
        simulation_clock.restart();
        auto stop1 = path_bus_stops.front();
        path_bus_stops.pop_front();
        auto stop2 = path_bus_stops.front();

        if (stop2.get_x() - stop1.get_x() <= 0)
        {
            bus.setTextureRect(left_view);
        }
        else
        {
            bus.setTextureRect(right_view);
        }

        bus_speed = sf::Vector2f((stop2.get_x() - stop1.get_x()) / (time.second * 60.f), (stop2.get_y() - stop1.get_y()) / (time.second * 60.f));
        status = "Travelling";
    }
    else if (time.first == 1 && simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        if (times.size() == 1)
        {
            status = "Dropping off passengers";
        }
        else
        {
            status = "Picking up and dropping off passengers";
        }

        times.pop_front();
        time = times.front();
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