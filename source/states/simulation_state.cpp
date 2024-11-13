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
#include "utils/calc_view.hpp"

SimulationState::SimulationState(GameDataRef data) : _data(data), first_time(true), status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue)), bus_stops_texture(sf::Image(sf::Vector2u(100, 50), sf::Color::White)), person_texture (sf::Image(sf::Vector2u(100, 50), sf::Color::White)), _view_dragger(std::make_unique<util::ViewDragger>(*_data->window))
{
    std::ifstream info_file("data/simulation_info.json");
    if (info_file.is_open())
    {
        nlohmann::json info_json;
        info_file >> info_json;
        
        for (auto info : info_json["simulation_info"])
        {
            this->_data->simulation_info.push_back(simulation_info_from_json(info));
        }

        info_file.close();
    }
}

SimulationState::SimulationState(GameDataRef data, std::vector<SimulationInfo> _simulation_info)
                : _data(data), first_time(true), status("Picking up passengers"), bus_texture(sf::Image(sf::Vector2u(200, 100), sf::Color::Blue))
{
}

SimulationInfo SimulationState::simulation_info_from_json(nlohmann::json j)
{
    Bus aux_bus = j["bus"];
    Bus *j_bus = &_data->player.get_bus(aux_bus.get_id());
    Employee aux_employee = j["employee"];
    Employee *j_employee = &_data->player.get_employee(aux_employee.get_id()); 

    std::vector<VisualElement> elements_path_vector;

    for (auto visual_element : j["elements_path"])
    {
        elements_path_vector.push_back(visual_element); 
    }

    std::vector<std::shared_ptr<VisualElement>> j_elements_path; 

    for (auto visual_element : elements_path_vector)
    {
        for (auto element : this->_data->city.get_visual_elements())
        {
            if (visual_element.get_id() == element->get_info()->get_id())
            {
                j_elements_path.push_back(element->get_info());
                break;
            }
        }
    }

    SimulationInfo simulation_info(j_bus, j_employee, {});
    
    simulation_info.elements_path = j_elements_path;
    simulation_info.time_state.first = j["time_state_first"];
    simulation_info.time_state.second = j["time_state_second"];
    simulation_info.path_index = j["path_index"];
    simulation_info.next_is_street = j["next_is_street"];
    simulation_info.route_completed =  j["route_completed"]; 
    simulation_info.previous_time = j["previous_time"];
    simulation_info.isVisible = j["isVisible"];
    simulation_info.projection_bus.setPosition(sf::Vector2f(j["position_x"], j["position_y"]));
    simulation_info.projection_bus_speed = sf::Vector2f(j["speed_x"], j["speed_y"]);

    for (auto time : j["times"])
    {
        simulation_info.track_times.push_back(time);
    }

    for (auto passengers : j["passengers_stop_first"])
    {
        simulation_info.passengers_per_stop.push_back(std::make_pair<int, int>(passengers, 0));
    }

    int i = 0; 
    for (auto &passengers : simulation_info.passengers_per_stop)
    {
        passengers.second = j["passengers_stop_second"].at(i);
        i++;
    }

    simulation_info.projection_clock.restart();

    return simulation_info;
}

void SimulationState::save()
{
    std::ofstream info_file("data/simulation_info.json");
    info_file << simulation_info_to_json().dump(4);
    info_file.close();
}

nlohmann::json SimulationState::simulation_info_to_json()
{
    simulation_clock.stop(); 

    nlohmann::json j; 

    nlohmann::json simulation_info = nlohmann::json::array(); 

    for (auto info : this->_data->simulation_info)
    {
        simulation_info.push_back(info.to_json());
    }

    j["simulation_info"] = simulation_info; 

    return j; 
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

    sf::View view;
    util::calc_view(*this->_data->window, view);

    int buttonHeight = 35;
    int buttonWidth = 150;

    auto exitButton = tgui::Button::create("Back to Menu");
    exitButton->setRenderer(tgui::Theme::getDefault()->getRenderer("RedButton"));
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

    init_bus();

    this->_map.load("assets/maps/demo.tmx");
}

void SimulationState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->gui.handleEvent(*event);
        this->_view_dragger->handleEvent(*event);
        if (event->is<sf::Event::Closed>())
        {
            save();
            this->_data->city.save(); 
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
    update_bus_stops();
}

// marks dt to not warn compiler
void SimulationState::draw_state(float dt __attribute__((unused)))
{
    // background color
    this->_data->window->clear(sf::Color::Black);

    static MapLayer layerZero(_map, 0);
    static MapLayer layerOne(_map, 1);
    static MapLayer layerTwo(_map, 2);
    static MapLayer layerThree(_map, 3);
    static MapLayer layerFour(_map, 4);
    static MapLayer layerFive(_map, 5);
    static MapLayer layerSix(_map, 6);
    static MapLayer layerSeven(_map, 7);
    static MapLayer layerEight(_map, 8);
    static MapLayer layerNine(_map, 9);

    // sf::Clock globalClock;
    // sf::Time duration = globalClock.restart();
    // layerZero.update(duration);
    layerZero.draw(*this->_data->window, sf::RenderStates::Default);
    layerOne.draw(*this->_data->window, sf::RenderStates::Default);
    layerTwo.draw(*this->_data->window, sf::RenderStates::Default);
    layerThree.draw(*this->_data->window, sf::RenderStates::Default);
    layerFour.draw(*this->_data->window, sf::RenderStates::Default);
    layerFive.draw(*this->_data->window, sf::RenderStates::Default);
    layerSix.draw(*this->_data->window, sf::RenderStates::Default);
    layerSeven.draw(*this->_data->window, sf::RenderStates::Default);
    layerEight.draw(*this->_data->window, sf::RenderStates::Default);
    layerNine.draw(*this->_data->window, sf::RenderStates::Default);

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

    for (auto info : this->_data->simulation_info)
    {
        if (info.isVisible)
        {
            this->_data->window->draw(info.projection_bus);
        }
    }

    draw_passengers(font);

    this->gui.draw();
    // Displays rendered objects
    this->_data->window->display();
}

void SimulationState::update_bus_stops()
{   
}

void SimulationState::draw_passengers(sf::Font font)
{
    int j = 0; 

    for (auto visual_element : this->_data->city.get_visual_elements())
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (bus_stop)
        {
            sf::Text amount(font); 

            amount.setString(bus_stop->get_name() + ": " + std::to_string(this->_data->city.get_current_passengers().at(j)) + " passengers");
            amount.setPosition(sf::Vector2f(bus_stop->get_x(), bus_stop->get_y()));
            amount.setCharacterSize(24); 

            this->_data->window->draw(amount); 

            j++;
        }
    }
}

void SimulationState::init_bus()
{
    sf::IntRect bus_rect(sf::Vector2i(0, 0), sf::Vector2i(48, 32));

    for (auto &info : this->_data->simulation_info)
    {
        if (info.time_state.first == -1 || info.previous_time != 0)
        {
            info.projection_bus.setTexture(bus_texture);
            info.projection_bus.setTextureRect(bus_rect);
            sf::FloatRect bounds = info.projection_bus.getLocalBounds();
            info.projection_bus.setOrigin(bounds.getCenter());
            if (info.time_state.first == -1)
            {
                info.projection_bus.setPosition(sf::Vector2f(info.elements_path.at(info.path_index)->get_x(), info.elements_path.at(info.path_index)->get_y()));
            }
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

        if (!this->_data->simulation_info.empty())
        {
            for (auto &info : this->_data->simulation_info)
            {
                info.projection_clock.restart(); 
            }
        }
        
        this->_data->city.run_simulation(this->_data->simulation_info);
        first_time = false;
    }

    if (simulation_clock.getElapsedTime().asSeconds() >= 1)
    {
        simulation_clock.restart();
        this->_data->city.run_simulation(this->_data->simulation_info);
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
            if (info.time_state.first == 1)
            {
                if (info.need_to_pay)
                {
                    continue; 
                }
                else
                {
                    auto messageBox = tgui::MessageBox::create();
                    messageBox->setTitle("Warning");
                    messageBox->setText(info.bus->get_name() + " broke down or ran out of fuel. \nYou need to pay 200 for a tow truck to pick it up");
                    messageBox->addButton("Pay");
                    messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f,
                                this->_data->window->getSize().y / 2 - 50.0f);
                    messageBox->setSize(400.0f, 100.f);
                    messageBox->onButtonPress([msgBox = messageBox.get(), this, &info](const tgui::String &button) {
                    this->_data->player.decrease_balance(200);
                    info.need_to_pay = false; 
                    info.time_state.first = 0; 
                    msgBox->getParent()->remove(msgBox->shared_from_this());
                    });
                    this->gui.add(messageBox);

                    info.need_to_pay = true; 
                }
            }
            else
            {
                info.employee->set_in_route(false);
                info.bus->set_in_route(false);
                info.isVisible = false;
                info.projection_bus.setTextureRect(sf::IntRect(sf::Vector2i(0.f, 0.f), sf::Vector2i(0.f, 0.f)));
                status = "Route completed";
                i++;
            }
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

    this->_data->achievement_manager.update(this->_data->player, this->_data->store, this->_data->simulation_info);
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
