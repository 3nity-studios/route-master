#include "states/bus_maintenance_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/management_state.hpp"
#include <string>

BusMaintenance::BusMaintenance(GameDataRef data, const int _bus_id) : _data(data), bus_id(_bus_id)
{
    // empty
}

void BusMaintenance::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/bus_maintenance.txt");

    this->_data->gui.get<tgui::Button>("cancel_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new ManagementState(this->_data)), false);
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

    const auto &item = this->_data->player.get_bus(bus_id);

    auto panel = tgui::ScrollablePanel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    panel->setPosition({10, 90});

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    float columnWidth = (this->_data->window->getSize().x - 30.0f) / 3;
    float rowHeight = 30;

    auto engineCheckbox = tgui::CheckBox::create();
    engineCheckbox->setWidgetName("EngineCheckbox");
    engineCheckbox->setText("Repair Engine");
    
    auto breaksCheckbox = tgui::CheckBox::create();
    breaksCheckbox->setWidgetName("BreaksCheckbox");
    breaksCheckbox->setText("Repair Breaks");
    
    auto tiresCheckbox = tgui::CheckBox::create();
    tiresCheckbox->setWidgetName("TiresCheckbox");
    tiresCheckbox->setText("Repair Tires");

    auto fuelCheckbox = tgui::CheckBox::create();
    fuelCheckbox->setWidgetName("FuelCheckbox");
    fuelCheckbox->setText("Refuel");

    this->_data->gui.get<tgui::Button>("repair_button")->onPress([this, item, engineCheckbox, breaksCheckbox, tiresCheckbox, fuelCheckbox] {
        this->_data->store.buy_bus_maintenance(item.get_id(),
                                            this->_data->player,
                                            engineCheckbox->isChecked(),
                                            breaksCheckbox->isChecked(),
                                            tiresCheckbox->isChecked(),
                                            fuelCheckbox->isChecked());
        this->_data->states.add_state(Engine::StateRef(new ManagementState(this->_data)), false);
    });

    // Draw bus details view for the bus
    const auto prices = this->_data->player.get_bus(this->bus_id).calc_maintenance_price();

    // Create labels for the item details
    auto itemLabel = tgui::Label::create(item.get_name());
    itemLabel->getRenderer()->setTextColor(tgui::Color::Black);
    itemLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    itemLabel->setSize({150, 35});
    itemLabel->getRenderer()->setTextSize(20);
    itemLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    itemLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(itemLabel, 0, 0);

    auto stateHeaderLabel = tgui::Label::create("State (%)");
    stateHeaderLabel->getRenderer()->setTextColor(tgui::Color::Black);
    stateHeaderLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    stateHeaderLabel->setSize({columnWidth, 30});
    stateHeaderLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    stateHeaderLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(stateHeaderLabel, 1, 1);

    auto priceHeaderLabel = tgui::Label::create("Price ($)");
    priceHeaderLabel->getRenderer()->setTextColor(tgui::Color::Black);
    priceHeaderLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    priceHeaderLabel->setSize({columnWidth, 30});
    priceHeaderLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    priceHeaderLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(priceHeaderLabel, 1, 2);

    auto engineStateLabel = tgui::Label::create(std::to_string(item.get_engine_state()) + "%");
    engineStateLabel->getRenderer()->setTextColor(tgui::Color::Black);
    engineStateLabel->setSize({columnWidth, 30});
    engineStateLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    engineStateLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(engineStateLabel, 2, 1);

    auto enginePriceLabel = tgui::Label::create("$" + std::to_string(prices.at(0)));
    enginePriceLabel->getRenderer()->setTextColor(tgui::Color::Black);
    enginePriceLabel->setSize({columnWidth, 30});
    enginePriceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    enginePriceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(enginePriceLabel, 2, 2);

    auto breaksStateLabel = tgui::Label::create(std::to_string(item.get_breaks_state()) + "%");
    breaksStateLabel->getRenderer()->setTextColor(tgui::Color::Black);
    breaksStateLabel->setSize({columnWidth, 30});
    breaksStateLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    breaksStateLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(breaksStateLabel, 3, 1);

    auto breaksPriceLabel = tgui::Label::create("$" + std::to_string(prices.at(1)));
    breaksPriceLabel->getRenderer()->setTextColor(tgui::Color::Black);
    breaksPriceLabel->setSize({columnWidth, 30});
    breaksPriceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    breaksPriceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(breaksPriceLabel, 3, 2);

    auto tiresStateLabel = tgui::Label::create(std::to_string(item.get_tires_state()) + "%");
    tiresStateLabel->getRenderer()->setTextColor(tgui::Color::Black);
    tiresStateLabel->setSize({columnWidth, 30});
    tiresStateLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    tiresStateLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(tiresStateLabel, 4, 1);

    auto tiresPriceLabel = tgui::Label::create("$" + std::to_string(prices.at(2)));
    tiresPriceLabel->getRenderer()->setTextColor(tgui::Color::Black);
    tiresPriceLabel->setSize({columnWidth, 30});
    tiresPriceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    tiresPriceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(tiresPriceLabel, 4, 2);

    auto fuelStateLabel = tgui::Label::create(std::to_string(item.get_fuel()) + "%");
    fuelStateLabel->getRenderer()->setTextColor(tgui::Color::Black);
    fuelStateLabel->setSize({columnWidth, 30});
    fuelStateLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    fuelStateLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(fuelStateLabel, 5, 1);

    auto fuelPriceLabel = tgui::Label::create("$" + std::to_string(prices.at(3)));
    fuelPriceLabel->getRenderer()->setTextColor(tgui::Color::Black);
    fuelPriceLabel->setSize({columnWidth, 30});
    fuelPriceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    fuelPriceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(fuelPriceLabel, 5, 2);

    auto totalPriceLabel = tgui::Label::create("Total: $" + std::to_string(prices.at(0) + prices.at(1) + prices.at(2)));
    totalPriceLabel->getRenderer()->setTextColor(tgui::Color::Black);
    totalPriceLabel->setSize({150, 30});
    totalPriceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    totalPriceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(totalPriceLabel, 6, 0);

    grid->addWidget(engineCheckbox, 2, 0);
    grid->addWidget(breaksCheckbox, 3, 0);
    grid->addWidget(tiresCheckbox, 4, 0);
    grid->addWidget(fuelCheckbox, 5, 0);

    panel->add(grid);
    this->_data->gui.add(panel);
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

    // Displays rendered objects
    this->_data->window->display();
}
