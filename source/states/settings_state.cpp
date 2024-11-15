#include "states/simulation_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <TGUI/Widgets/EditBox.hpp>
#include <string>
#include <cmath>
#include "states/main_menu_state.hpp"
#include "states/settings_state.hpp"
#include "utils/calc_view.hpp"

SettingsState::SettingsState(GameDataRef data) : _data(data)
{
    // empty
}

void SettingsState::init_state()
{ 
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/settings_screen.txt");

    util::calc_view(*this->_data->window, this->_view);

    this->_data->gui.get<tgui::Button>("back_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), true);
    });

    this->_data->gui.get<tgui::Button>("save_button")->onPress([this] {
        save_settings();
    });

    this->_data->gui.get<tgui::EditBox>("inp_name")->setText(this->_data->player.get_name());
    this->_data->gui.get<tgui::CheckBox>("chk_music")->setChecked(this->_data->is_music_on);
}

void SettingsState::update_inputs()
{
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
            if (keyPress->code == sf::Keyboard::Key::Escape)
            {
                this->_data->states.remove_state();
                break;
            }
        }
    }
}

void SettingsState::update_state(float dt __attribute__((unused)))
{
}

void SettingsState::draw_state(float dt __attribute__((unused)))
{
    this->_data->window->clear(sf::Color::Black);
    this->_data->gui.draw();
    this->_data->window->display();
}

void SettingsState::save_settings()
{
    auto chosen_name = this->_data->gui.get<tgui::EditBox>("inp_name")->getText();
    auto chosen_music = this->_data->gui.get<tgui::CheckBox>("chk_music")->isChecked();
    this->_data->is_music_on = chosen_music;
    this->_data->player.set_name(chosen_name.toStdString());
}