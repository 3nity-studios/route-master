#include "states/inventory_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

InventoryState::InventoryState(GameDataRef data) : _data(data)
{
    
}

void InventoryState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/inventory.txt");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    });

    auto buses = this->_data->player.get_buses();
    for (int i = 0; i < buses.size(); ++i)
    {
        auto &item = buses[i];

        std::cout << item.get_name();
        
        // Create a button for doing maintenance to the bus
        auto buyButton = tgui::Button::create();
        buyButton->setWidgetName(item.get_id() + "RepairButton");
        buyButton->setPosition({700.0f, 120.0f + (i * 30.0f)});
        buyButton->setText("Repair " + item.get_name());
        buyButton->onPress([this, item] {
            this->_data->store.buy_bus_maintenance(item.get_id(), this->_data->player, true, true, true);
        });
        this->_data->gui.add(buyButton);
    }
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

    sf::RectangleShape rectangle(sf::Vector2f(5, 80));
    rectangle.setPosition({5, 85});
    rectangle.setFillColor(sf::Color(255, 255, 255, 128));
    rectangle.setSize(sf::Vector2f(690, 400));
    this->_data->window->draw(rectangle);

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

    // Draw table headers
    sf::Text headerName(font);
    headerName.setString("Item Name");
    headerName.setCharacterSize(20);
    headerName.setFillColor(sf::Color::Black);
    headerName.setStyle(sf::Text::Bold);
    headerName.setPosition({10.0f, 90.0f});
    this->_data->window->draw(headerName);

    sf::Text headerEngineState(font);
    headerEngineState.setString("Engine State");
    headerEngineState.setCharacterSize(20);
    headerEngineState.setFillColor(sf::Color::Black);
    headerEngineState.setStyle(sf::Text::Bold);
    headerEngineState.setPosition({130.0f, 90.0f});
    this->_data->window->draw(headerEngineState);

    sf::Text headerBreaksState(font);
    headerBreaksState.setString("Breaks State");
    headerBreaksState.setCharacterSize(20);
    headerBreaksState.setFillColor(sf::Color::Black);
    headerBreaksState.setStyle(sf::Text::Bold);
    headerBreaksState.setPosition({280.0f, 90.0f});
    this->_data->window->draw(headerBreaksState);

    sf::Text headerTiresState(font);
    headerTiresState.setString("Tires State");
    headerTiresState.setCharacterSize(20);
    headerTiresState.setFillColor(sf::Color::Black);
    headerTiresState.setStyle(sf::Text::Bold);
    headerTiresState.setPosition({430.0f, 90.0f});
    this->_data->window->draw(headerTiresState);

    sf::Text headerFuel(font);
    headerFuel.setString("Fuel Level");
    headerFuel.setCharacterSize(20);
    headerFuel.setFillColor(sf::Color::Black);
    headerFuel.setStyle(sf::Text::Bold);
    headerFuel.setPosition({580.0f, 90.0f});
    this->_data->window->draw(headerFuel);

    auto buses = this->_data->player.get_buses();
    for (int i = 0; i < buses.size(); ++i)
    {
        const auto &item = buses[i];

        // Create a text object for the item name
        sf::Text itemText(font);
        itemText.setString(item.get_name());
        itemText.setCharacterSize(20);
        itemText.setFillColor(sf::Color::Black);
        itemText.setStyle(sf::Text::Regular);
        itemText.setPosition({10.0f, 120.0f + (i * 30.0f)});
        this->_data->window->draw(itemText);

        // Create a text object for the bus engine state
        sf::Text engineStateText(font);
        engineStateText.setString(std::to_string(item.get_engine_state()));
        engineStateText.setCharacterSize(20);
        engineStateText.setFillColor(sf::Color::Black);
        engineStateText.setStyle(sf::Text::Regular);
        engineStateText.setPosition({130.0f, 120.0f + (i * 30.0f)});
        this->_data->window->draw(engineStateText);

        // Create a text object for the bus breaks state
        sf::Text breaksStateText(font);
        breaksStateText.setString(std::to_string(item.get_breaks_state()));
        breaksStateText.setCharacterSize(20);
        breaksStateText.setFillColor(sf::Color::Black);
        breaksStateText.setStyle(sf::Text::Regular);
        breaksStateText.setPosition({280.0f, 120.0f + (i * 30.0f)});
        this->_data->window->draw(breaksStateText);

        // Create a text object for the bus tires state
        sf::Text tiresStateText(font);
        tiresStateText.setString(std::to_string(item.get_tires_state()));
        tiresStateText.setCharacterSize(20);
        tiresStateText.setFillColor(sf::Color::Black);
        tiresStateText.setStyle(sf::Text::Regular);
        tiresStateText.setPosition({430.0f, 120.0f + (i * 30.0f)});
        this->_data->window->draw(tiresStateText);

        // Create a text object for the bus fuel level
        sf::Text fuelText(font);
        fuelText.setString(std::to_string(item.get_fuel()));
        fuelText.setCharacterSize(20);
        fuelText.setFillColor(sf::Color::Black);
        fuelText.setStyle(sf::Text::Regular);
        fuelText.setPosition({580.0f, 120.0f + (i * 30.0f)});
        this->_data->window->draw(fuelText);
    }

    // Displays rendered objects
    this->_data->window->display();
}
