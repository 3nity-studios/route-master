#include "states/inventory_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

InventoryState::InventoryState(GameDataRef data) : _data(data)
{
    for (int i = 0; i < this->_data->player.get_buses().size(); ++i)
    {
        auto &item = this->_data->player.get_buses()[i];

        // Create a button for doing maintenance to the bus
        auto buyButton = tgui::Button::create();
        buyButton->setWidgetName(item.get_id() + "RepairButton");
        buyButton->setPosition({450.0f, 90.0f + (i * 30.0f)});
        buyButton->setText("Repair " + item.get_name());
        buyButton->onPress([this, item] {
            this->_data->store.buy_bus_maintenance(item.get_id(), this->_data->player, true, true, true);
        });
        this->_data->gui.add(buyButton);
    }
}

void InventoryState::init_state()
{
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/inventory.txt");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    });
}

void InventoryState::update_inputs()
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
void InventoryState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void InventoryState::draw_state(float dt __attribute__((unused)))
{
    this->_data->gui.draw();

    // write text
    sf::Font font("assets/fonts/joystix.ttf");

    // throws error if can't load font
    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        // error...
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    // Create a text object for the player name
    sf::Text playerNameText(font);
    playerNameText.setString("Player: " + this->_data->player.get_name());
    playerNameText.setCharacterSize(20);
    playerNameText.setFillColor(sf::Color::Black);
    playerNameText.setStyle(sf::Text::Regular);

    // Set the position of the player name text
    playerNameText.setPosition({this->_data->window->getSize().x - 200.0f, 10.0f});
    this->_data->window->draw(playerNameText);

    // Create a text object for the player balance
    sf::Text playerBalanceText(font);
    playerBalanceText.setString("Balance: $" + std::to_string(this->_data->player.get_balance()));
    playerBalanceText.setCharacterSize(20);
    playerBalanceText.setFillColor(sf::Color::Black);
    playerBalanceText.setStyle(sf::Text::Regular);

    // Set the position of the player balance text
    playerBalanceText.setPosition({this->_data->window->getSize().x - 200.0f, 40.0f});
    this->_data->window->draw(playerBalanceText);

    for (int i = 0; i < this->_data->player.get_buses().size(); ++i)
    {
        const auto &item = this->_data->player.get_buses()[i];

        // Create a text object for the item name
        sf::Text itemText(font);
        itemText.setString(item.get_name());
        itemText.setCharacterSize(20);
        itemText.setFillColor(sf::Color::Black);
        itemText.setStyle(sf::Text::Regular);

        // Set the position of the text
        itemText.setPosition({10.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(itemText);

        // Create a text object for the bus engine state
        sf::Text engineStateText(font);
        engineStateText.setString("Engine State: " + std::to_string(item.get_engine_state()));
        engineStateText.setCharacterSize(20);
        engineStateText.setFillColor(sf::Color::Black);
        engineStateText.setStyle(sf::Text::Regular);

        // Set the position of the engine state text
        engineStateText.setPosition({150.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(engineStateText);

        // Create a text object for the bus breaks state
        sf::Text breaksStateText(font);
        breaksStateText.setString("Breaks State: " + std::to_string(item.get_breaks_state()));
        breaksStateText.setCharacterSize(20);
        breaksStateText.setFillColor(sf::Color::Black);
        breaksStateText.setStyle(sf::Text::Regular);

        // Set the position of the breaks state text
        breaksStateText.setPosition({300.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(breaksStateText);

        // Create a text object for the bus tires state
        sf::Text tiresStateText(font);
        tiresStateText.setString("Tires State: " + std::to_string(item.get_tires_state()));
        tiresStateText.setCharacterSize(20);
        tiresStateText.setFillColor(sf::Color::Black);
        tiresStateText.setStyle(sf::Text::Regular);

        // Set the position of the tires state text
        tiresStateText.setPosition({450.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(tiresStateText);

        // Create a text object for the bus fuel level
        sf::Text fuelText(font);
        fuelText.setString("Fuel: " + std::to_string(item.get_fuel()));
        fuelText.setCharacterSize(20);
        fuelText.setFillColor(sf::Color::Black);
        fuelText.setStyle(sf::Text::Regular);

        // Set the position of the fuel text
        fuelText.setPosition({600.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(fuelText);
    }

    // Displays rendered objects
    this->_data->window->display();
}
