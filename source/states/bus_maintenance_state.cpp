#include "states/bus_maintenance_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/inventory_state.hpp"
#include <string>

BusMaintenance::BusMaintenance(GameDataRef data, const int _bus_id) : _data(data), bus_id(bus_id)
{
}

void BusMaintenance::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/bus_maintenance.txt");

    this->_data->gui.get<tgui::Button>("cancel_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new InventoryState(this->_data)), false);
    });

    const auto &item = this->_data->player.get_bus(this->bus_id);

    auto engineCheckbox = tgui::CheckBox::create();
    engineCheckbox->setWidgetName("EngineCheckbox");
    engineCheckbox->setPosition({10.0f, 120.0f});
    engineCheckbox->setText("Repair Engine");
    this->_data->gui.add(engineCheckbox);
    
    auto breaksCheckbox = tgui::CheckBox::create();
    breaksCheckbox->setWidgetName("BreaksCheckbox");
    breaksCheckbox->setPosition({10.0f, 150.0f});
    breaksCheckbox->setText("Repair Breaks");
    this->_data->gui.add(breaksCheckbox);

    auto tiresCheckbox = tgui::CheckBox::create();
    tiresCheckbox->setWidgetName("TiresCheckbox");
    tiresCheckbox->setPosition({10.0f, 180.0f});
    tiresCheckbox->setText("Repair Tires");
    this->_data->gui.add(tiresCheckbox);

    this->_data->gui.get<tgui::Button>("repair_button")->onPress([this, item, engineCheckbox, breaksCheckbox, tiresCheckbox] {
        this->_data->store.buy_bus_maintenance(item.get_id(),
                                            this->_data->player,
                                            engineCheckbox->isChecked(),
                                            breaksCheckbox->isChecked(),
                                            tiresCheckbox->isChecked());
        this->_data->states.add_state(Engine::StateRef(new InventoryState(this->_data)), false);
    });
}

void BusMaintenance::update_inputs()
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
void BusMaintenance::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void BusMaintenance::draw_state(float dt __attribute__((unused)))
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

    // Draw bus details view for the bus
    const auto &item = this->_data->player.get_bus(this->bus_id);
    const auto prices = this->_data->player.get_bus(this->bus_id).calc_maintenance_price();

    // Create a text object for the item name
    sf::Text itemText(font);
    itemText.setString(item.get_name());
    itemText.setCharacterSize(20);
    itemText.setFillColor(sf::Color::Black);
    itemText.setStyle(sf::Text::Bold);
    itemText.setPosition({10.0f, 90.0f});
    this->_data->window->draw(itemText);

    // Header for the state
    sf::Text stateHeaderText(font);
    stateHeaderText.setString("State (%)");
    stateHeaderText.setCharacterSize(20);
    stateHeaderText.setFillColor(sf::Color::Black);
    stateHeaderText.setStyle(sf::Text::Bold);
    stateHeaderText.setPosition({200.0f, 90.0f});
    this->_data->window->draw(stateHeaderText);

    // Header for the price
    sf::Text priceHeaderText(font);
    priceHeaderText.setString("Price ($)");
    priceHeaderText.setCharacterSize(20);
    priceHeaderText.setFillColor(sf::Color::Black);
    priceHeaderText.setStyle(sf::Text::Bold);
    priceHeaderText.setPosition({400.0f, 90.0f});
    this->_data->window->draw(priceHeaderText);

    sf::Text engineStateText(font);
    engineStateText.setString(std::to_string(item.get_engine_state()) + "%");
    engineStateText.setCharacterSize(20);
    engineStateText.setFillColor(sf::Color::Black);
    engineStateText.setStyle(sf::Text::Regular);
    engineStateText.setPosition({200.0f, 120.0f});
    this->_data->window->draw(engineStateText);

    sf::Text enginePriceText(font);
    enginePriceText.setString("$" + std::to_string(prices.at(0)));
    enginePriceText.setCharacterSize(20);
    enginePriceText.setFillColor(sf::Color::Black);
    enginePriceText.setStyle(sf::Text::Regular);
    enginePriceText.setPosition({400.0f, 120.0f});
    this->_data->window->draw(enginePriceText);

    sf::Text breaksStateText(font);
    breaksStateText.setString(std::to_string(item.get_breaks_state()) + "%");
    breaksStateText.setCharacterSize(20);
    breaksStateText.setFillColor(sf::Color::Black);
    breaksStateText.setStyle(sf::Text::Regular);
    breaksStateText.setPosition({200.0f, 150.0f});
    this->_data->window->draw(breaksStateText);

    sf::Text breaksPriceText(font);
    breaksPriceText.setString("$" + std::to_string(prices.at(1)));
    breaksPriceText.setCharacterSize(20);
    breaksPriceText.setFillColor(sf::Color::Black);
    breaksPriceText.setStyle(sf::Text::Regular);
    breaksPriceText.setPosition({400.0f, 150.0f});
    this->_data->window->draw(breaksPriceText);

    sf::Text tiresStateText(font);
    tiresStateText.setString(std::to_string(item.get_tires_state()) + "%");
    tiresStateText.setCharacterSize(20);
    tiresStateText.setFillColor(sf::Color::Black);
    tiresStateText.setStyle(sf::Text::Regular);
    tiresStateText.setPosition({200.0f, 180.0f});
    this->_data->window->draw(tiresStateText);

    sf::Text tiresPriceText(font);
    tiresPriceText.setString("$" + std::to_string(prices.at(2)));
    tiresPriceText.setCharacterSize(20);
    tiresPriceText.setFillColor(sf::Color::Black);
    tiresPriceText.setStyle(sf::Text::Regular);
    tiresPriceText.setPosition({400.0f, 180.0f});
    this->_data->window->draw(tiresPriceText);

    sf::Text totalPrice(font);
    totalPrice.setString("Total: $" + std::to_string(prices.at(0) + prices.at(1) + prices.at(2)));
    totalPrice.setCharacterSize(20);
    totalPrice.setFillColor(sf::Color::Black);
    totalPrice.setStyle(sf::Text::Regular);
    totalPrice.setPosition({10.0f, 210.0f});
    this->_data->window->draw(totalPrice);

    // Displays rendered objects
    this->_data->window->display();
}
