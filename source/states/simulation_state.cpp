#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include <cmath>

float calc_distance(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

SimulationState::SimulationState(GameDataRef data) : _data(data), first_time(true), status("Picking up passengers"), bus_sim(Bus(1, "Bus 125", 32, std::list<Passenger>{}, 5)), driver_sim(Employee(1, "John", "Doe", 25, 50, 5, 0)), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus_stops_texture(sf::Image(sf::Vector2u(100, 50), sf::Color::White)), bus(bus_texture)
{
    BusStop stop1(1, "Stop1", {10, 11, 10}, 2.0, 5.0, 3.0, 3.0, 2.0, 350.f, 5.f);
    BusStop stop2(2, "Stop2", {10, 11, 10}, 3.0, 10.0, 3.0, 3.0, 2.0, 500.f, 5.f);
    BusStop stop3(3, "Stop3", {10, 8, 9}, 15.0, 15.0, 3.0, 3.0, 2.0, 750.f, 5.f);
    BusStop stop4(4, "Stop4", {10, 11, 10}, 15.0, 15.0, 3.0, 3.0, 2.0, 350.f, 250.f);
    BusStop stop5(5, "Stop5", {11, 10, 11}, 15.0, 15.0, 3.0, 3.0, 2.0, 600.f, 250.f);
    BusStop stop6(6, "Stop6", {10, 11, 10}, 15.0, 15.0, 3.0, 3.0, 2.0, 600.f, 500.f);

    TrafficLight light1(7, std::vector<std::pair<StreetConnectionIDs, bool>>{std::make_pair<StreetConnectionIDs, bool>(std::make_pair<int, int>(4,5), true)}, 10, 450.f, 250.f);

    VisualElement curve1(8, 180.f, 200.f);
    VisualElement curve2(9, 800.f, 500.f);

    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);
    city.add_bus_stop(stop4);
    city.add_traffic_light(light1);
    city.add_bus_stop(stop5);
    city.add_bus_stop(stop6);
    city.add_curve(curve1);
    city.add_curve(curve2);

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
    : _data(data), times(_simulation_times), first_time(true), bus_sim (_simulation_bus), driver_sim(_simulation_driver),
      status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus(bus_texture)
{
}

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

    sf::View view(sf::FloatRect({0.f, 0.f}, {1000.f, 600.f}));
    this->_data->window->setView(view);
    init_bus_stops();
    init_bus();
}

/// the last known mouse position
sf::Vector2i previous_mouse_position;
/// whether we are dragging or not
bool dragging = false;

void SimulationState::update_inputs()
{
    sf::RenderTarget &target{*this->_data->window};
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);

        if (event->is<sf::Event::Closed>())
        {
            this->_data->window->close();
            break;
        }

        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonPressed>())
        {
            // if mouse button is pressed start dragging
            if (keyPress->button == sf::Mouse::Button::Right)
            {
                dragging = true;
                return;
            }
        }
        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonReleased>())
        {
            // if mouse button is released stop draggin
            if (keyPress->button == sf::Mouse::Button::Right)
            {
                dragging = false;
                return;
            }
        }
        // if dragging mouse
        if (const auto *keyPress = event->getIf<sf::Event::MouseMoved>())
        {
            // get mouse position
            const sf::Vector2i mouse_position{keyPress->position.x, keyPress->position.y};
            // if dragging, move view
            if (dragging)
            {
                // calculate how far mouse has moved in view
                const auto delta =
                    target.mapPixelToCoords(mouse_position) - target.mapPixelToCoords(previous_mouse_position);
                // apply negatively to view
                auto view = target.getView();
                view.move(-delta);
                target.setView(view);
            }
            // update previous mouse position
            previous_mouse_position = mouse_position;
            return;
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

    tmx::Map map;
    map.load("assets/maps/demo.tmx");

    MapLayer layerZero(map, 0);
    MapLayer layerOne(map, 1);
    MapLayer layerTwo(map, 2);

    sf::Clock globalClock;
    sf::Time duration = globalClock.restart();
    layerZero.update(duration);
    this->_data->window->draw(layerZero);
    this->_data->window->draw(layerOne);

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

    text.setPosition(sf::Vector2f(300.f, 500.f));

    // inside the main loop, between window.clear() and window.display()
    this->_data->window->draw(text);

    // END SAMPLE RENDER CODE

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
    // set the string to display
    text2.setString("\nDriver: " + driver_sim.get_name() + " " + driver_sim.get_last_name() + "\n" + "Bus: " + bus_sim.get_name());

    // set the character size
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
    std::uniform_real_distribution<float> dis (lower, upper);

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
            bus_stop.setTextureRect(sf::IntRect(sf::Vector2i(617,200), sf::Vector2i(197,104)));
            bus_stop.setPosition(sf::Vector2f(visual_element->get_info()->get_x(), visual_element->get_info()->get_y()));
            bus_stop.setScale(sf::Vector2<float>(0.5, 0.5));
            bus_stops.push_back(bus_stop);

            std::list<sf::CircleShape> passenger_list;

            for (int i = 0; i < stop->get_passenger_list().size(); i++)
            {
                std::uniform_real_distribution<float> dis(bus_stop.getPosition().x, bus_stop.getLocalBounds().size.x);

                sf::CircleShape passenger; 

                passenger.setPosition(sf::Vector2f(generateRandom(bus_stop.getPosition().x, bus_stop.getPosition().x + (0.5*bus_stop.getLocalBounds().size.x)), bus_stop.getPosition().y + 60));

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
    sf::IntRect bus_texture(sf::Vector2i(8,5), sf::Vector2i(358,657));

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

    if ((time.first == 0 || time.first == 2 || time.first == 3) && simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        if(time.first == 0)
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

        sf::Angle rotation = sf::radians(M_PI - (atan((stop2->get_x() - stop1->get_x()) / (stop2->get_y() - stop1->get_y()))));

        bus.setRotation(rotation);

        bus_speed = sf::Vector2f((stop2->get_x() - stop1->get_x()) / (time.second * 60.f), (stop2->get_y() - stop1->get_y()) / (time.second * 60.f));
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
