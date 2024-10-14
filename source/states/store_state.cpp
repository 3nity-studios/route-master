#include "states/store_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

StoreState::StoreState(GameDataRef data) : _data(data)
{
    // empty
}

void StoreState::update_items_to_show(tgui::String filter)
{
    items_to_show.clear();

    if (filter == "All")
    {
        for (const auto &item : this->_data->store.get_inventory())
        {
            items_to_show.append(item);
        }
    }
    else
    {
        ItemType type;
        if (filter == "Buses")
        {
            type = ItemType::Bus;
        }
        else if (filter == "Employees")
        {
            type = ItemType::Employee;
        }

        for (const auto &item : this->_data->store.get_inventory())
        {
            if (item.get_category() == type)
            {
                items_to_show.append(item);
            }
        }
    }

    int i = 0;
    for (const auto &item : items_to_show)
    {
        // Create a button for buying the item
        auto buyButton = tgui::Button::create();
        buyButton->setPosition({450.0f, 90.0f + (i * 30.0f)});
        buyButton->setText("Buy " + item.get_name());
        buyButton->onPress([this, item] { this->_data->store.buy_item(this->_data->player, item.get_id(), 1); });
        this->_data->gui.add(buyButton);
        ++i;
    }
}

void StoreState::init_state()
{
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/store.txt");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    });

    tgui::ComboBox::Ptr comboBox = tgui::ComboBox::create();
    comboBox->setWidgetName("filter_combobox");
    comboBox->setPosition(50, 50);
    comboBox->addItem("All");
    comboBox->addItem("Buses");
    comboBox->addItem("Employees");
    comboBox->onItemSelect([this, comboBox] {
        this->update_items_to_show(comboBox->getSelectedItem());
    });
    comboBox->setSelectedItem("All");
    this->_data->gui.add(comboBox);
}

void StoreState::update_inputs()
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
void StoreState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void StoreState::draw_state(float dt __attribute__((unused)))
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

    int i = 0;
    for (const auto &item : items_to_show)
    {
        // Create a text object for the item name
        sf::Text itemText(font);
        itemText.setString(item.get_name());
        itemText.setCharacterSize(20);
        itemText.setFillColor(sf::Color::Black);
        itemText.setStyle(sf::Text::Regular);

        // Set the position of the text
        itemText.setPosition({10.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(itemText);

        // Create a text object for the item price
        sf::Text priceText(font);
        priceText.setString("Price: " + std::to_string(item.get_price()));
        priceText.setCharacterSize(20);
        priceText.setFillColor(sf::Color::Black);
        priceText.setStyle(sf::Text::Regular);

        // Set the position of the price text
        priceText.setPosition({150.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(priceText);

        // Create a text object for the item amount
        sf::Text amountText(font);
        amountText.setString("Amount: " + std::to_string(item.get_amount()));
        amountText.setCharacterSize(20);
        amountText.setFillColor(sf::Color::Black);
        amountText.setStyle(sf::Text::Regular);

        // Set the position of the amount text
        amountText.setPosition({300.0f, 90.0f + (i * 30.0f)});
        this->_data->window->draw(amountText);

        ++i;
    }

    // Displays rendered objects
    this->_data->window->display();
}
