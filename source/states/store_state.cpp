#include "states/store_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

StoreState::StoreState(GameDataRef data) : _data(data)
{
    // empty
}

void StoreState::update_items_to_show(tgui::String filter)
{
    this->_data->gui.remove(this->_data->gui.get<tgui::ScrollablePanel>("StorePanel"));
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

    this->_data->gui.add(create_store_panel());
}

void StoreState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Store");
    
    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.remove_state();
    });

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
    
    // Displays rendered objects
    this->_data->window->display();
}

tgui::ScrollablePanel::Ptr StoreState::create_store_panel()
{
    auto panel = tgui::ScrollablePanel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    panel->setPosition({10, 120});

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    // Add headers
    auto nameLabel = tgui::RichTextLabel::create("Item Name");
    nameLabel->setSize({200, 30});
    nameLabel->getRenderer()->setTextColor(sf::Color::Black);
    nameLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    nameLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    nameLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(nameLabel, 0, 0);

    auto priceLabel = tgui::RichTextLabel::create("Price");
    priceLabel->setSize({120, 30});
    priceLabel->getRenderer()->setTextColor(sf::Color::Black);
    priceLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    priceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    priceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(priceLabel, 0, 1);

    auto amountLabel = tgui::RichTextLabel::create("Amount");
    amountLabel->setSize({120, 30});
    amountLabel->getRenderer()->setTextColor(sf::Color::Black);
    amountLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    amountLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    amountLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(amountLabel, 0, 2);

    // Add items to the grid
    for (int i = 0; i < items_to_show.size(); ++i)
    {
        auto &item = items_to_show[i];

        auto itemName = tgui::RichTextLabel::create(item.get_name());
        itemName->setSize({120, 30});
        itemName->getRenderer()->setTextColor(sf::Color::Black);
        itemName->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        itemName->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(itemName, i+1, 0);

        auto itemPrice = tgui::RichTextLabel::create(std::to_string(item.get_price()));
        itemPrice->setSize({120, 30});
        itemPrice->getRenderer()->setTextColor(sf::Color::Black);
        itemPrice->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        itemPrice->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(itemPrice, i+1, 1);

        auto itemAmount = tgui::RichTextLabel::create(std::to_string(item.get_amount()));
        itemAmount->setSize({120, 30});
        itemAmount->getRenderer()->setTextColor(sf::Color::Black);
        itemAmount->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        itemAmount->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(itemAmount, i+1, 2);

        // Create a button for buying the item
        auto buyButton = tgui::Button::create();
        buyButton->setSize({120, 25});
        if(item.get_category() == ItemType::Employee)
        {
            buyButton->setText("Hire");
        }
        else
        {
            buyButton->setText("Buy");
        }
        buyButton->onPress([this, item] { 
            this->_data->store.buy_item(this->_data->player, item.get_id(), 1);
            this->update_items_to_show(this->_data->gui.get<tgui::ComboBox>("filter_combobox")->getSelectedItem());
        });
        grid->addWidget(buyButton, i+1, 3);
    }

    panel->add(grid);
    panel->setWidgetName("StorePanel");
    return panel;
}