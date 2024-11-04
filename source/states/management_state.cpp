#include "states/management_state.hpp"
#include "states/bus_maintenance_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

ManagementState::ManagementState(GameDataRef data) : _data(data)
{
    
}

void ManagementState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");
    this->_data->gui.get<tgui::Label>("title")->setText("Management");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    });

    auto payroll_panel = create_payroll_panel();
    auto inventory_panel = create_inventory_panel();

    auto selector = tgui::TabContainer::create();
    selector->setTabsHeight(25);
    selector->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    selector->setTabAlignment(tgui::TabContainer::TabAlign::Top);
    selector->setPosition({10.0f, 90.0f});
    selector->addTab("Inventory", true)->add(inventory_panel);
    selector->addTab("Payroll", false)->add(payroll_panel);
    this->_data->gui.add(selector);
}

void ManagementState::update_inputs()
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
void ManagementState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void ManagementState::draw_state(float dt __attribute__((unused)))
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



    
    // Displays rendered objects
    this->_data->window->display();
}

tgui::ScrollablePanel::Ptr ManagementState::create_payroll_panel()
{
    auto panel = tgui::ScrollablePanel::create();
    // auto panel = tgui::ScrollablePanel::create({this->_data->window->getSize().x - 20.0f, 400.0f});
    // panel->setPosition({10.0f, 90.0f});
    panel->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255, 200));

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    int columnWidth = 120;
    int rowHeight = 35;

    // Add headers
    auto nameLabel = tgui::RichTextLabel::create("Name");
    nameLabel->setSize({120, 35});
    nameLabel->getRenderer()->setTextColor(sf::Color::Black);
    nameLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(nameLabel, 0, 0);

    auto lastNameLabel = tgui::RichTextLabel::create("Last Name");
    lastNameLabel->setSize({120, 35});
    lastNameLabel->getRenderer()->setTextColor(sf::Color::Black);
    lastNameLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(lastNameLabel, 0, 1);

    auto workTimeLabel = tgui::RichTextLabel::create("Accumulated Work Time");
    workTimeLabel->setSize({150, 35});
    workTimeLabel->getRenderer()->setTextColor(sf::Color::Black);
    workTimeLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(workTimeLabel, 0, 2);

    auto hourlyRateLabel = tgui::RichTextLabel::create("Hourly Rate");
    hourlyRateLabel->setSize({120, 35});
    hourlyRateLabel->getRenderer()->setTextColor(sf::Color::Black);
    hourlyRateLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(hourlyRateLabel, 0, 3);

    auto paycheckLabel = tgui::RichTextLabel::create("Paycheck");
    paycheckLabel->setSize({120, 35});
    paycheckLabel->getRenderer()->setTextColor(sf::Color::Black);
    paycheckLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(paycheckLabel, 0, 4);

    auto employees = this->_data->player.get_employees();
    for (int i = 0; i < employees.size(); ++i)
    {
        auto &employee = employees[i];

        auto nameLabel = tgui::RichTextLabel::create(employee.get_name());
        nameLabel->setSize({120, 35});
        nameLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(nameLabel, i + 1, 0);

        auto lastNameLabel = tgui::RichTextLabel::create(employee.get_last_name());
        lastNameLabel->setSize({120, 35});
        lastNameLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(lastNameLabel, i + 1, 1);

        auto workTimeLabel = tgui::RichTextLabel::create(std::to_string(employee.get_accumulated_work_time()));
        workTimeLabel->setSize({120, 35});
        workTimeLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(workTimeLabel, i + 1, 2);

        auto hourlyRateLabel = tgui::RichTextLabel::create(std::to_string(employee.get_hourly_rate()));
        hourlyRateLabel->setSize({120, 35});
        hourlyRateLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(hourlyRateLabel, i + 1, 3);

        auto paycheckLabel = tgui::RichTextLabel::create(std::to_string(employee.get_hourly_rate() * employee.get_accumulated_work_time()));
        paycheckLabel->setSize({120, 35});
        paycheckLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(paycheckLabel, i + 1, 4);

         // Create a button for doing maintenance to the bus
        auto buyButton = tgui::Button::create();
        buyButton->setWidgetName(employee.get_id() + "PayButton");
        buyButton->setText("Pay " + employee.get_name());
        buyButton->onPress([this, employee] {
            this->_data->store.pay_employee(employee.get_id(), this->_data->player);
        });
        grid->addWidget(buyButton, i + 1, 5);
    }

    panel->add(grid);
    panel->setWidgetName("Payroll");
    return panel;
}

tgui::ScrollablePanel::Ptr ManagementState::create_inventory_panel()
{
    auto panel = tgui::ScrollablePanel::create();
    // auto panel = tgui::ScrollablePanel::create({this->_data->window->getSize().x - 20.0f, 400.0f});
    // panel->setPosition({10.0f, 500.0f});
    panel->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255, 200));

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    // Add headers
    auto busLabel = tgui::RichTextLabel::create("Bus");
    busLabel->setSize({120, 35});
    busLabel->getRenderer()->setTextColor(sf::Color::Black);
    busLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(busLabel, 0, 0);

    auto engineLabel = tgui::RichTextLabel::create("Engine");
    engineLabel->setSize({120, 35});
    engineLabel->getRenderer()->setTextColor(sf::Color::Black);
    engineLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(engineLabel, 0, 1);

    auto breaksLabel = tgui::RichTextLabel::create("Breaks");
    breaksLabel->setSize({120, 35});
    breaksLabel->getRenderer()->setTextColor(sf::Color::Black);
    breaksLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(breaksLabel, 0, 2);

    auto tiresLabel = tgui::RichTextLabel::create("Tires");
    tiresLabel->setSize({120, 35});
    tiresLabel->getRenderer()->setTextColor(sf::Color::Black);
    tiresLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(tiresLabel, 0, 3);

    auto fuelLabel = tgui::RichTextLabel::create("Fuel");
    fuelLabel->setSize({120, 35});
    fuelLabel->getRenderer()->setTextColor(sf::Color::Black);
    fuelLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    grid->addWidget(fuelLabel, 0, 4);

    auto buses = this->_data->player.get_buses();
    for (int i = 0; i < buses.size(); ++i)
    {
        auto &bus = buses[i];

        auto busLabel = tgui::RichTextLabel::create(bus.get_name());
        busLabel->setSize({120, 35});
        busLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(busLabel, i + 1, 0);

        auto engineLabel = tgui::RichTextLabel::create(std::to_string(bus.get_engine_state()));
        engineLabel->setSize({120, 35});
        engineLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(engineLabel, i + 1, 1);

        auto breaksLabel = tgui::RichTextLabel::create(std::to_string(bus.get_breaks_state()));
        breaksLabel->setSize({120, 35});
        breaksLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(breaksLabel, i + 1, 2);

        auto tiresLabel = tgui::RichTextLabel::create(std::to_string(bus.get_tires_state()));
        tiresLabel->setSize({120, 35});
        tiresLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(tiresLabel, i + 1, 3);

        auto fuelLabel = tgui::RichTextLabel::create(std::to_string(bus.get_fuel()));
        fuelLabel->setSize({120, 35});
        fuelLabel->getRenderer()->setTextColor(sf::Color::Black);
        grid->addWidget(fuelLabel, i + 1, 4);

        // Create a button for doing maintenance to the bus
        auto repairButton = tgui::Button::create();
        repairButton->setWidgetName(bus.get_id() + "RepairButton");
        repairButton->setText("Repair " + bus.get_name());
        repairButton->onPress([this, bus] {
            this->_data->states.add_state(Engine::StateRef(new BusMaintenance(this->_data, bus.get_id())), false);
        });
        grid->addWidget(repairButton, i + 1, 5);
    }

    panel->add(grid);
    panel->setWidgetName("Inventory");
    return panel;
}