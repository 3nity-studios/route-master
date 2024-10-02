#include "states/main_menu_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

#include <iostream>

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{
    for (int i = 4; i < 23; i++)
    {
        times.push_back(std::make_pair<int, int>(i%2, i*1));
    }
    first_time = true;
    state = "Picking up passengers";
}

void MainMenuState::init_state()
{
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/main_menu.txt");
    init_bus_stops(10);
    init_bus();
}

void MainMenuState::update_inputs()
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
                this->_data->window->close();
                break;
            }
        }
    }
}

// marks dt to not warn compiler
void MainMenuState::update_state(float dt __attribute__((unused)))
{
    update_bus();
}

// marks dt to not warn compiler
void MainMenuState::draw_state(float dt __attribute__((unused)))
{
    // just for fun, heres hello world text
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
    text.setString(state);

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

    // Displays rendered objects
    this->_data->window->display();
}

void MainMenuState::init_bus_stops(int bus_stops_amount)
{ 
    float distance = 5.f;

    for (int i = 0; i < bus_stops_amount; i++)
    {
        sf::RectangleShape bus_stop;

        bus_stop.setSize(sf::Vector2f(25.f, 25.f));

        bus_stop.setPosition(sf::Vector2f(distance, 50.f));

        bus_stops.push_back(bus_stop);

        distance += 75.f;
    }
}

void MainMenuState::init_bus()
{
    bus.setSize(sf::Vector2f(25.f, 10.f));

    bus.setPosition(sf::Vector2f(5.f, 20.f));

    bus.setFillColor(sf::Color::Blue);
}

void MainMenuState::update_bus()
{
    if (times.empty())
    {
        return; 
    }

    if (first_time)
    {
        simulation_clock.restart();
        first_time = false;
    }

    auto time = times.front();

    std::cout<<time.first<<std::endl;
    std::cout<<simulation_clock.getElapsedTime().asSeconds()<<std::endl;

    if (time.first == 0 && simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        times.pop_front();
        time = times.front();
        simulation_clock.restart();
        bus_speed = sf::Vector2f(75.f/(time.second * 60.f), 0.f);
        state = "Traveling";
    }
    else if (time.first == 1 && simulation_clock.getElapsedTime().asSeconds() >= time.second)
    {
        times.pop_front();
        time = times.front();
        simulation_clock.restart();
        bus_speed = sf::Vector2f(0.f,0.f);
        state = "Picking up passengers";
    }

    bus.move(bus_speed);
}