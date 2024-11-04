#include "states/main_menu_state.hpp"
#include "states/simulation_state.hpp"
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
    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
            this->_data->window->close();
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
    this->_data->gui.draw();
    // Displays rendered objects
    this->_data->window->display();
}
