#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

SimulationState::SimulationState(GameDataRef data) : _data(data), first_time(true), status("Picking up passengers"), bus_sim(Bus(1, "Bus 125", 32, std::list<Passenger>{}, 5)), driver_sim(Employee ("John", "Doe", 32, 12, 0))
{
    BusStop stop1(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    BusStop stop2(2, "Stop2", 10.0, 10.0, 3.0, 3.0, 2.0);
    BusStop stop3(3, "Stop3", 15.0, 15.0, 3.0, 3.0, 2.0);

    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops();

    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f, 0.05f);

    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);

    StreetArcList path;

    for (int i = 1; i < 3; i++)
    {
        for (auto street : city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path.append(street);
            }
        }
    }

    auto time = city.run_simulation(bus_sim, driver_sim, 10, path);

    this->times.clear();

    set_simulation_parameters(time, 3);
}

SimulationState::SimulationState(GameDataRef data, std::list<std::pair<int, int>> _simulation_times,
                                 int _simulation_stops_number, Bus _simulation_bus, Employee _simulation_driver)
    : _data(data), times(_simulation_times), stops_number(_simulation_stops_number), first_time(true), bus_sim (_simulation_bus), driver_sim(_simulation_driver),
      status("Picking up passengers")
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
    text.setCharacterSize(24); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text.setPosition({(static_cast<float>(this->_data->window->getSize().x / 2)) - (text.getLocalBounds().size.x / 2),
                      (static_cast<float>(this->_data->window->getSize().y / 2)) - (text.getLocalBounds().size.y / 2)});

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
    text2.setCharacterSize(24); // in pixels, not points!

    // set the color
    text2.setFillColor(sf::Color::White);

    // set the text style
    text2.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text2.setPosition(
        {(static_cast<float>(this->_data->window->getSize().x / 2)) - (text2.getLocalBounds().size.x / 2),
         (static_cast<float>(this->_data->window->getSize().y / 2)) - (text2.getLocalBounds().size.y / 2) + 24});

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
    float distance = 5.f;

    for (int i = 0; i < stops_number; i++)
    {
        sf::RectangleShape bus_stop;

        bus_stop.setSize(sf::Vector2f(25.f, 25.f));

        bus_stop.setPosition(sf::Vector2f(distance, 50.f));

        bus_stops.push_back(bus_stop);

        distance += 75.f;
    }
}

void SimulationState::init_bus()
{
    bus.setSize(sf::Vector2f(25.f, 10.f));

    bus.setPosition(sf::Vector2f(5.f, 20.f));

    bus.setFillColor(sf::Color::Blue);
}

void SimulationState::update_bus()
{
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
        bus_speed = sf::Vector2f(75.f / (time.second * 60.f), 0.f);
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

void SimulationState::set_simulation_parameters(std::list<std::pair<int, int>> _times, int _stops_number)
{
    times = _times;
    stops_number = _stops_number;
}