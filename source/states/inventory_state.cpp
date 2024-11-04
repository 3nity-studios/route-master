#include "states/inventory_state.hpp"
#include "states/bus_maintenance_state.hpp"
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
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Inventory");

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
        // buyButton->onPress([this, item] {
        //     this->_data->store.buy_bus_maintenance(item.get_id(), this->_data->player, true, true, true);
        // });
        buyButton->onPress([this, item] {
            this->_data->states.add_state(Engine::StateRef(new BusMaintenance(this->_data, item.get_id())), false);
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

    // write text
    sf::Font font("assets/fonts/joystix.ttf");

    // throws error if can't load font
    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        // error...
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    // Create a label for the player name
    auto playerInfoLabel = tgui::Label::create();
    playerInfoLabel->setText("Player: " + this->_data->player.get_name() + 
                            "\nBalance: $" + std::to_string(this->_data->player.get_balance()));
    playerInfoLabel->getRenderer()->setTextColor(sf::Color::White);
    playerInfoLabel->getRenderer()->setTextOutlineColor(sf::Color::Black);
    playerInfoLabel->getRenderer()->setTextOutlineThickness(2);
    playerInfoLabel->setTextSize(16);
    playerInfoLabel->setPosition({this->_data->window->getSize().x - 200.0f, 20.0f});
    this->_data->gui.add(playerInfoLabel);

    auto listView = tgui::ListView::create();
    listView->setSize({690.0f, 400.0f});
    listView->setPosition({5, 85});
    listView->setHeaderHeight(30);
    listView->addColumn("Bus", 120, tgui::HorizontalAlignment::Center);
    listView->addColumn("Engine", 120, tgui::HorizontalAlignment::Center);
    listView->addColumn("Breaks", 120, tgui::HorizontalAlignment::Center);
    listView->addColumn("Tires", 120, tgui::HorizontalAlignment::Center);
    listView->addColumn("Fuel", 120, tgui::HorizontalAlignment::Center);

    auto buses = this->_data->player.get_buses();
    for (const auto &item : buses)
    {
        listView->addItem({item.get_name(), 
                           std::to_string(item.get_engine_state()), 
                           std::to_string(item.get_breaks_state()), 
                           std::to_string(item.get_tires_state()), 
                           std::to_string(item.get_fuel())});
    }

    this->_data->gui.add(listView);
    // Displays rendered objects
    this->_data->window->display();
}
