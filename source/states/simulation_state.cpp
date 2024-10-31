#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include <cmath>

float calc_distance(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

SimulationState::SimulationState(GameDataRef data) : _data(data), first_time(true), status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus_stops_texture(sf::Image(sf::Vector2u(100, 50), sf::Color::White)), person_texture (sf::Image(sf::Vector2u(100, 50), sf::Color::White))
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

    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);
    city.add_bus_stop(stop4);
    city.add_traffic_light(light1);
    city.add_bus_stop(stop5);
    city.add_bus_stop(stop6);
    city.add_curve(curve1);
    city.add_curve(curve2);
    city.add_curve(curve3);
    city.add_curve(curve4);

    city.initialize_bus_stops();

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

    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    city.add_street(street3, 3, 8);
    city.add_street(street4, 8, 9);
    city.add_street(street5, 9, 4);
    city.add_street(street6, 4, 7);
    city.add_street(street7, 7, 5);
    city.add_street(street8, 5, 10);
    city.add_street(street9, 10, 11);
    city.add_street(street10, 11, 6);

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

    StreetArcList path2;

    for (int i = 6; i < 11; i++)
    {
        for (auto street : city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path2.append(street);
            }
        }
    }

    Bus bus_sim(1, "Bus 1", 10, {}, 5);
    Employee driver_sim(1, "John", "Doe", 33, 12, 0);

    Bus bus_sim2(2, "Bus 2", 15, {}, 5);
    Employee driver_sim2(2, "Rodrigo", "Hernandez", 23, 12, 0);

    SimulationInfo simulation(bus_sim, driver_sim, path);
    SimulationInfo simulation2(bus_sim2, driver_sim2, path2);

    this->simulation_info.push_back(simulation);
    this->simulation_info.push_back(simulation2);

    city.update();
    city.update_passengers();
}

SimulationState::SimulationState(GameDataRef data, std::vector<SimulationInfo> _simulation_info)
                : _data(data), simulation_info(_simulation_info), first_time(true), status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue))
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

    if (!person_texture.loadFromFile("assets/img/person.png"))
    {
        throw GameException("Couldn't find file: assets/img/person.png");
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
    update_bus_stops();
}

// marks dt to not warn compiler
void SimulationState::draw_state(float dt __attribute__((unused)))
{
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

    for (auto bus_stop : bus_stops)
    {
        this->_data->window->draw(bus_stop);
    }

    draw_passengers();

    for (auto info : simulation_info)
    {
        this->_data->window->draw(info.projection_bus);
    }

    sf::Text text2(font);
    // set the string to display
    text2.setString("\nDriver: " + simulation_info.front().employee.get_name() + " " + simulation_info.front().employee.get_last_name() + "\n" + "Bus: " + simulation_info.front().bus.get_name());

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

void SimulationState::init_bus_stops()
{
    for (auto visual_element : city.get_visual_elements())
    {
        auto stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (stop)
        {
            sf::Sprite bus_stop(bus_stops_texture);
            bus_stop.setTextureRect(sf::IntRect(sf::Vector2i(617,200), sf::Vector2i(197,104)));
            bus_stop.setPosition(sf::Vector2f(stop->get_x(), stop->get_y()));
            bus_stop.setScale(sf::Vector2<float>(0.5, 0.5));
            bus_stops.push_back(bus_stop);
        }
    }
}

void SimulationState::update_bus_stops()
{   
}

void SimulationState::draw_passengers()
{
    int j = 0; 
    int k = 0; 
    sf::IntRect person_select(sf::Vector2i(0, 0), sf::Vector2i(15, 20));

    for (auto visual_element : city.get_visual_elements())
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (bus_stop)
        {
            float passenger_distance = 0; 
            float vertical_distance = 35; 
            k = 0; 

            for (int i = 0; i < city.get_current_passengers().at(j); i++)
            {
                switch (k)
                {
                    case 0: 
                        person_select.position = sf::Vector2i(0, 0);
                        break;
                    case 1: 
                        person_select.position = sf::Vector2i(15, 0);
                        break;
                    case 2: 
                        person_select.position = sf::Vector2i(30, 0);
                        break;
                    default:
                        person_select.position = sf::Vector2i(45,0);
                    break;
                }

                sf::Sprite person(person_texture);
                person.setPosition(sf::Vector2f(bus_stop->get_x() + passenger_distance, bus_stop->get_y() + vertical_distance));
                person.setTextureRect(person_select);
                this->_data->window->draw(person);

                passenger_distance = passenger_distance + 15.f;

                if (passenger_distance > 90.f)
                {
                    passenger_distance = 0.f; 
                    vertical_distance += 10.f;
                }

                if (k == 3)
                {
                    k = 0; 
                }
                else
                {
                    k++;
                }
            }

            j++;
        }
    }
}

void SimulationState::init_bus()
{
    sf::IntRect bus_rect(sf::Vector2i(8,5), sf::Vector2i(358,657));

    for (auto &info : simulation_info)
    {
        info.projection_bus.setTexture(bus_texture);
        info.projection_bus.setTextureRect(bus_rect);
        sf::FloatRect bounds = info.projection_bus.getLocalBounds();
        info.projection_bus.setOrigin(bounds.getCenter());
        info.projection_bus.rotate(sf::degrees(270));
        info.projection_bus.setPosition(sf::Vector2f(info.elements_path.front()->get_x() + 35.f, info.elements_path.front()->get_y() + 85.f));
        info.projection_bus.setScale(sf::Vector2<float>(0.1, 0.1));
    }
}

void SimulationState::update_bus()
{
    if (first_time)
    {
        simulation_clock.restart();
        city.run_simulation(simulation_info);
        current_time++;
        first_time = false;
    }

    if (simulation_clock.getElapsedTime().asSeconds() >= 1)
    {
        simulation_clock.restart();
        city.run_simulation(simulation_info);
        current_time++;
    }
    else
    {
        for (auto &info : simulation_info)
        {
            info.projection_bus.move(info.projection_bus_speed);
        }

        return;
    }

    for (auto &info : simulation_info)
    {
        if (info.route_completed)
        {
            status = "Route completed";
            continue;
        }

        auto time = info.time_state;

        if (time.first == 0 || time.first == 2 || time.first == 3)
        {
            if (time.first == 0)
            {
                status = "Picking up and dropping off passengers";
            }
            else if (time.first == 2)
            {
                status = "Waiting in Traffic Light";
            }

            info.projection_bus_speed = sf::Vector2f(0.f, 0.f);
        }
        else if (time.first == 1)
        {
            auto stop1 = info.elements_path.at(info.path_index - 1);
            auto stop2 = info.elements_path.at(info.path_index);

            sf::Angle rotation = sf::radians(M_PI - (atan((stop2->get_x() - stop1->get_x()) / (stop2->get_y() - stop1->get_y()))));
            info.projection_bus.setRotation(rotation);

            info.projection_bus_speed = sf::Vector2f((stop2->get_x() - stop1->get_x()) / (time.second * 60.f), (stop2->get_y() - stop1->get_y()) / (time.second * 60.f));
            status = "Travelling";
        }

        info.projection_bus.move(info.projection_bus_speed);
    }
}

void SimulationState::set_simulation_info(std::vector<SimulationInfo> _simulation_info)
{
    simulation_info = _simulation_info;
}

std::vector<SimulationInfo> SimulationState::get_simulation_info()
{
    return simulation_info;
}

SimulationState::~SimulationState()
{
}
