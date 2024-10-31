#include "states/main_menu_state.hpp"
#include "states/simulation_state.hpp"
#include "states/store_state.hpp"
#include "states/inventory_state.hpp"
#include "states/payroll_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{
}

void MainMenuState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/main_menu.txt");
    this->_data->gui.get<tgui::Button>("play_button")->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new SimulationState(this->_data)), false);
        });
    this->_data->gui.get<tgui::Button>("store_button")->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new StoreState(this->_data)), false);
        });
    this->_data->gui.get<tgui::Button>("inventory_button")->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new InventoryState(this->_data)), false);
        });
    this->_data->gui.get<tgui::Button>("payroll_button")->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new PayrollState(this->_data)), false);
        });
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
    this->_data->gui.draw();
    // Displays rendered objects
    this->_data->window->display();
}
