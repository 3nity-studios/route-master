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
    for (const auto &item : items_to_show)
    {
        this->_data->gui.remove(this->_data->gui.get<tgui::Button>(item.get_id() + "BuyButton"));
    }
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
        buyButton->setWidgetName(item.get_id() + "BuyButton");
        buyButton->setPosition({450.0f, 120.0f + (i * 30.0f)});
        buyButton->setText("Buy " + item.get_name());
        buyButton->onPress([this, item] { 
            this->_data->store.buy_item(this->_data->player, item.get_id(), 1);
            this->update_items_to_show(this->_data->gui.get<tgui::ComboBox>("filter_combobox")->getSelectedItem());
        });
        this->_data->gui.add(buyButton);
        ++i;
    }
}

void StoreState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Store");
    
    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    });

    tgui::ComboBox::Ptr comboBox = tgui::ComboBox::create();
    comboBox->setWidgetName("filter_combobox");
    comboBox->setPosition(600.0f, 90.0f);
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

    // Create a ListView for displaying items
    auto listView = tgui::ListView::create();
    listView->setSize({440.0f, 400.0f});
    listView->setPosition({10, 90});
    listView->setHeaderHeight(30);
    listView->addColumn("Item Name", 175, tgui::HorizontalAlignment::Center);
    listView->addColumn("Price", 100, tgui::HorizontalAlignment::Center);
    listView->addColumn("Amount", 100, tgui::HorizontalAlignment::Center);
    listView->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);

    // Add items to the ListView
    for (const auto &item : items_to_show)
    {
        listView->addItem({item.get_name(), std::to_string(item.get_price()), std::to_string(item.get_amount())});
    }

    // Add the ListView to the GUI
    this->_data->gui.add(listView);

    // Displays rendered objects
    this->_data->window->display();
}
