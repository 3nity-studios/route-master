#include "states/map_viewer_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

MapViewerState::MapViewerState(GameDataRef data) : _data(data)
{
}

void MapViewerState::init_state()
{
}

void MapViewerState::update_inputs()
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
void MapViewerState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void MapViewerState::draw_state(float dt __attribute__((unused)))
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
    text.setString("Hello world!");

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
    tmx::Map map;
    map.load("assets/maps/demo.tmx");

    MapLayer layerZero(map, 0);
    // MapLayer layerOne(map, 1);
    // MapLayer layerTwo(map, 2);

    sf::Clock globalClock;
    sf::Time duration = globalClock.restart();
    layerZero.update(duration);
    this->_data->window->clear(sf::Color::Black);
    this->_data->window->draw(layerZero);
    // this->_data->window->draw(layerOne);
    // this->_data->window->draw(layerTwo);
    this->_data->window->display();
    // Displays rendered objects
    this->_data->window->display();
}
