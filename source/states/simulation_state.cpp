#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include <cmath>
#include "states/bus_select_state.hpp"
#include "states/main_menu_state.hpp"
#include "states/simulation_state.hpp"
#include "states/store_state.hpp"
#include "states/route_list_state.hpp"
#include "states/management_state.hpp"
#include "states/stats_state.hpp"
#include "states/achievements_state.hpp"

SimulationState::SimulationState(GameDataRef data) : _data(data), first_time(true), status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus_stops_texture(sf::Image(sf::Vector2u(100, 50), sf::Color::White)), person_texture (sf::Image(sf::Vector2u(100, 50), sf::Color::White))
{
}

SimulationState::SimulationState(GameDataRef data, std::vector<SimulationInfo> _simulation_info)
                : _data(data), first_time(true), status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue))
{
}

void SimulationState::init_state()
{ 
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->gui.setWindow(*this->_data->window);

    if (!bus_texture.loadFromFile("assets/img/buses/encava_base.png"))
    {
        throw GameException("Couldn't find file: assets/img/buses/encava_base.png");
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

    int buttonHeight = 35;
    int buttonWidth = 150;

    auto exitButton = tgui::Button::create("Back to Menu");
    exitButton->setPosition(10, this->_data->window->getSize().y - (buttonHeight + 5));
    exitButton->setSize(buttonWidth, buttonHeight);
    exitButton->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), true);
    });
    this->gui.add(exitButton);

    auto achievementsButton = tgui::Button::create("Achievements");
    achievementsButton->setPosition(10, this->_data->window->getSize().y - 2*(buttonHeight + 5));
    achievementsButton->setSize(buttonWidth, buttonHeight);
    achievementsButton->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new AchievementsState(this->_data)), false);
    });
    this->gui.add(achievementsButton);

    auto statsButton = tgui::Button::create("Statistics");
    statsButton->setPosition(10, this->_data->window->getSize().y - 3*(buttonHeight + 5));
    statsButton->setSize(buttonWidth, buttonHeight);
    statsButton->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new StatsState(this->_data)), false);
    });
    this->gui.add(statsButton);

    auto storeButton = tgui::Button::create("Store");
    storeButton->setPosition(10, this->_data->window->getSize().y - 4*(buttonHeight + 5));
    storeButton->setSize(buttonWidth, buttonHeight);
    storeButton->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new StoreState(this->_data)), false);
    });
    this->gui.add(storeButton);

    auto managementButton = tgui::Button::create("Management");
    managementButton->setPosition(10, this->_data->window->getSize().y - 5*(buttonHeight + 5));
    managementButton->setSize(buttonWidth, buttonHeight);
    managementButton->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new ManagementState(this->_data)), false);
    });
    this->gui.add(managementButton);

    auto designRouteButton = tgui::Button::create("Routes");
    designRouteButton->setPosition(10, this->_data->window->getSize().y - 6*(buttonHeight + 5));
    designRouteButton->setSize(buttonWidth, buttonHeight);
    designRouteButton->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new RouteListState(this->_data)), false);
    });
    this->gui.add(designRouteButton);

    auto sendBusButton = tgui::Button::create("Send Bus");
    sendBusButton->setPosition(10, this->_data->window->getSize().y - 7
    *(buttonHeight + 5));
    sendBusButton->setSize(buttonWidth, buttonHeight);
    sendBusButton->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new BusSelectState(this->_data)), false);
    });
    this->gui.add(sendBusButton);


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
        this->gui.handleEvent(*event);

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

    for (auto info : this->_data->simulation_info)
    {
        if (info.isVisible)
        {
            this->_data->window->draw(info.projection_bus);
        }
    }

    this->_data->window->draw(layerTwo);

    this->gui.draw();
    // Displays rendered objects
    this->_data->window->display();
}

void SimulationState::init_bus_stops()
{
    for (auto visual_element : this->_data->city.get_visual_elements())
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

    for (auto visual_element : this->_data->city.get_visual_elements())
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (bus_stop)
        {
            float passenger_distance = 0; 
            float vertical_distance = 35; 
            k = 0; 

            for (int i = 0; i < this->_data->city.get_current_passengers().at(j); i++)
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
    sf::IntRect bus_rect(sf::Vector2i(0, 0), sf::Vector2i(48, 32));

    for (auto &info : this->_data->simulation_info)
    {
        if (info.time_state.first == -1)
        {
            info.projection_bus.setTexture(bus_texture);
            info.projection_bus.setTextureRect(bus_rect);
            sf::FloatRect bounds = info.projection_bus.getLocalBounds();
            info.projection_bus.setOrigin(bounds.getCenter());
            info.projection_bus.setPosition(sf::Vector2f(info.elements_path.front()->get_x() + 35.f, info.elements_path.front()->get_y() + 85.f));
            info.projection_bus.setScale(sf::Vector2<float>(2.0, 2.0)); // twice cuz tileset rendering in 32x32
        }
    }
}

void SimulationState::update_bus()
{
    sf::IntRect right_view(sf::Vector2i(0, 0), sf::Vector2i(48, 32));
    sf::IntRect left_view(sf::Vector2i(0, 32), sf::Vector2i(48, 32));
    sf::IntRect down_view(sf::Vector2i(48, 0), sf::Vector2i(16, 48));
    sf::IntRect up_view(sf::Vector2i(64, 0), sf::Vector2i(16, 48));
    if (first_time)
    {
        simulation_clock.restart();
        this->_data->city.run_simulation(this->_data->simulation_info);
        current_time++;
        first_time = false;
    }

    if (simulation_clock.getElapsedTime().asSeconds() >= 1)
    {
        simulation_clock.restart();
        this->_data->city.run_simulation(this->_data->simulation_info);
        current_time++;
    }
    else
    {
        for (auto &info : this->_data->simulation_info)
        {
            info.projection_bus.move(info.projection_bus_speed);
        }

        return;
    }

    int i = 0; 

    for (auto &info : this->_data->simulation_info)
    {
        if (info.route_completed)
        {
            info.employee->set_in_route(false);
            info.bus->set_in_route(false);
            info.isVisible = false;
            info.projection_bus.setTextureRect(sf::IntRect(sf::Vector2i(0.f, 0.f), sf::Vector2i(0.f, 0.f)));
            status = "Route completed";
            i++;
            continue;
        }

        if (!info.isVisible)
        {
            i++;
            check_is_visible(info);
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

            //encava changes according to greatest difference between axis(es?)
            if (abs(stop2->get_x() - stop1->get_x()) > abs(stop2->get_y() - stop1->get_y()))
            {
                if (stop2->get_x() - stop1->get_x() <= 0)
                {
                    info.projection_bus.setTextureRect(left_view);
                }
                else
                {
                    info.projection_bus.setTextureRect(right_view);
                }
            }
            else
            {
                if (stop2->get_y() - stop1->get_y() <= 0)
                {
                    info.projection_bus.setTextureRect(up_view);
                }
                else
                {
                    info.projection_bus.setTextureRect(down_view);
                }
            }

            info.projection_bus_speed = sf::Vector2f((stop2->get_x() - stop1->get_x()) / (time.second * 60.f), (stop2->get_y() - stop1->get_y()) / (time.second * 60.f));
            status = "Travelling";
        }

        manage_collisions(info, i);

        i++;

        info.projection_bus.move(info.projection_bus_speed);
    }
}

void SimulationState::set_simulation_info(std::vector<SimulationInfo> _simulation_info)
{
    this->_data->simulation_info = _simulation_info;
}

std::vector<SimulationInfo> SimulationState::get_simulation_info()
{
    return this->_data->simulation_info;
}

void SimulationState::resume_state()
{
    init_bus(); 

    simulation_clock.start();

    int i = 0; 

    for (auto &info : this->_data->simulation_info)
    {
        if (i != this->_data->simulation_info.size() - 1)
        {
            if (info.projection_bus.getGlobalBounds().findIntersection(this->_data->simulation_info.back().projection_bus.getGlobalBounds()) != std::nullopt)
            {
                this->_data->simulation_info.back().isVisible = false; 
            }
        }
       
        info.projection_clock.start();
        i++;
    }
}

void SimulationState::pause_state()
{
    simulation_clock.stop();

    for (auto &info : this->_data->simulation_info)
    {
        info.projection_clock.stop();
    }
}

void SimulationState::add_simulation_info(SimulationInfo _simulation_info)
{
    this->_data->simulation_info.push_back(_simulation_info);
}

void SimulationState::manage_collisions(SimulationInfo &info, int i)
{
    int j = 0; 

    for (auto &info2: this->_data->simulation_info)
    {
        if (i == j)
        {
            continue; 
        }

        if (info2.projection_bus.getGlobalBounds().contains(info.calc_vector_tip()))
        {
            info.projection_bus_speed = sf::Vector2f(0.f, 0.f);
            info.projection_clock.stop();
        }
        else
        {
            info.projection_clock.start();
        }

        j++;
    }
}

void SimulationState::check_is_visible(SimulationInfo &info_to_check)
{
    for (auto info : this->_data->simulation_info)
    {
        if (info.projection_bus.getGlobalBounds().findIntersection(info_to_check.projection_bus.getGlobalBounds()) == std::nullopt)
        {
            info_to_check.isVisible = true;
        }
    }
}

SimulationState::~SimulationState()
{
}
