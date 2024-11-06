#include "states/management_state.hpp"
#include "states/bus_maintenance_state.hpp"
#include "states/bus_improvements_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

ManagementState::ManagementState(GameDataRef data) : _data(data)
{
    // empty
}

void ManagementState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");
    this->_data->gui.get<tgui::Label>("title")->setText("Management");

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
    
    // Displays rendered objects
    this->_data->window->display();
}

tgui::ScrollablePanel::Ptr ManagementState::create_payroll_panel()
{
    auto panel = tgui::ScrollablePanel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    float columnWidth = (this->_data->window->getSize().x - 30.0f) / 6;
    float rowHeight = 30;

    // Add headers
    auto nameLabel = tgui::RichTextLabel::create("Name");
    nameLabel->setSize({columnWidth, 30});
    nameLabel->getRenderer()->setTextColor(sf::Color::Black);
    nameLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    nameLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    nameLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(nameLabel, 0, 0);

    auto lastNameLabel = tgui::RichTextLabel::create("Last Name");
    lastNameLabel->setSize({columnWidth, 30});
    lastNameLabel->getRenderer()->setTextColor(sf::Color::Black);
    lastNameLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    lastNameLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    lastNameLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(lastNameLabel, 0, 1);

    auto workTimeLabel = tgui::RichTextLabel::create("Total Work Hours");
    workTimeLabel->setSize({columnWidth, 35});
    workTimeLabel->getRenderer()->setTextColor(sf::Color::Black);
    workTimeLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    workTimeLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    workTimeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(workTimeLabel, 0, 2);

    auto hourlyRateLabel = tgui::RichTextLabel::create("Hourly Rate");
    hourlyRateLabel->setSize({columnWidth, 35});
    hourlyRateLabel->getRenderer()->setTextColor(sf::Color::Black);
    hourlyRateLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    hourlyRateLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    hourlyRateLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(hourlyRateLabel, 0, 3);

    auto paycheckLabel = tgui::RichTextLabel::create("Paycheck");
    paycheckLabel->setSize({columnWidth, 30});
    paycheckLabel->getRenderer()->setTextColor(sf::Color::Black);
    paycheckLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    paycheckLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    paycheckLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(paycheckLabel, 0, 4);

    auto employees = this->_data->player.get_employees();
    for (int i = 0; i < employees.size(); ++i)
    {
        auto &employee = employees[i];

        auto nameLabel = tgui::RichTextLabel::create(employee.get_name());
        nameLabel->setSize({columnWidth, 30});
        nameLabel->getRenderer()->setTextColor(sf::Color::Black);
        nameLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        nameLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(nameLabel, i + 1, 0);

        auto lastNameLabel = tgui::RichTextLabel::create(employee.get_last_name());
        lastNameLabel->setSize({columnWidth, 30});
        lastNameLabel->getRenderer()->setTextColor(sf::Color::Black);
        lastNameLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        lastNameLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(lastNameLabel, i + 1, 1);

        auto workTimeLabel = tgui::RichTextLabel::create(std::to_string(employee.get_total_work_hours()));
        workTimeLabel->setSize({columnWidth, 30});
        workTimeLabel->getRenderer()->setTextColor(sf::Color::Black);
        workTimeLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        workTimeLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(workTimeLabel, i + 1, 2);

        auto hourlyRateLabel = tgui::RichTextLabel::create(std::to_string(employee.get_hourly_rate()));
        hourlyRateLabel->setSize({columnWidth, 30});
        hourlyRateLabel->getRenderer()->setTextColor(sf::Color::Black);
        hourlyRateLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        hourlyRateLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(hourlyRateLabel, i + 1, 3);

        auto paycheckLabel = tgui::RichTextLabel::create(std::to_string(employee.get_hourly_rate() * employee.get_total_work_hours()));
        paycheckLabel->setSize({columnWidth, 30});
        paycheckLabel->getRenderer()->setTextColor(sf::Color::Black);
        paycheckLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        paycheckLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(paycheckLabel, i + 1, 4);

         // Create a button for doing maintenance to the bus
        auto payButton = tgui::Button::create();
        payButton->setSize({columnWidth, 25});
        payButton->setWidgetName(employee.get_id() + "PayButton");
        payButton->setText("Pay");
        payButton->onPress([this, employee] {
            this->_data->store.pay_employee(employee.get_id(), this->_data->player);
        });
        grid->addWidget(payButton, i + 1, 5);
    }

    panel->add(grid);
    panel->setWidgetName("Payroll");
    return panel;
}

tgui::ScrollablePanel::Ptr ManagementState::create_inventory_panel()
{
    auto panel = tgui::ScrollablePanel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    float columnWidth = (this->_data->window->getSize().x - 30.0f) / 7;
    float rowHeight = 30;

    // Add headers
    auto busLabel = tgui::RichTextLabel::create("Bus");
    busLabel->setSize({columnWidth, 30});
    busLabel->getRenderer()->setTextColor(sf::Color::Black);
    busLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    busLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    busLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(busLabel, 0, 0);

    auto engineLabel = tgui::RichTextLabel::create("Engine");
    engineLabel->setSize({columnWidth, 30});
    engineLabel->getRenderer()->setTextColor(sf::Color::Black);
    engineLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    engineLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    engineLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(engineLabel, 0, 1);

    auto breaksLabel = tgui::RichTextLabel::create("Breaks");
    breaksLabel->setSize({columnWidth, 30});
    breaksLabel->getRenderer()->setTextColor(sf::Color::Black);
    breaksLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    breaksLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    breaksLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(breaksLabel, 0, 2);

    auto tiresLabel = tgui::RichTextLabel::create("Tires");
    tiresLabel->setSize({columnWidth, 30});
    tiresLabel->getRenderer()->setTextColor(sf::Color::Black);
    tiresLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    tiresLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    tiresLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(tiresLabel, 0, 3);

    auto fuelLabel = tgui::RichTextLabel::create("Fuel");
    fuelLabel->setSize({columnWidth, 30});
    fuelLabel->getRenderer()->setTextColor(sf::Color::Black);
    fuelLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    fuelLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    fuelLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(fuelLabel, 0, 4);

    auto buses = this->_data->player.get_buses();
    for (int i = 0; i < buses.size(); ++i)
    {
        auto &bus = buses[i];

        auto busLabel = tgui::RichTextLabel::create(bus.get_name());
        busLabel->setSize({columnWidth, 30});
        busLabel->getRenderer()->setTextColor(sf::Color::Black);
        busLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        busLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(busLabel, i + 1, 0);

        auto engineLabel = tgui::RichTextLabel::create(std::to_string(bus.get_engine_state()));
        engineLabel->setSize({columnWidth, 30});
        engineLabel->getRenderer()->setTextColor(sf::Color::Black);
        engineLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        engineLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(engineLabel, i + 1, 1);

        auto breaksLabel = tgui::RichTextLabel::create(std::to_string(bus.get_breaks_state()));
        breaksLabel->setSize({columnWidth, 30});
        breaksLabel->getRenderer()->setTextColor(sf::Color::Black);
        breaksLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        breaksLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(breaksLabel, i + 1, 2);

        auto tiresLabel = tgui::RichTextLabel::create(std::to_string(bus.get_tires_state()));
        tiresLabel->setSize({columnWidth, 30});
        tiresLabel->getRenderer()->setTextColor(sf::Color::Black);
        tiresLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        tiresLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(tiresLabel, i + 1, 3);

        auto fuelLabel = tgui::RichTextLabel::create(std::to_string(bus.get_fuel()));
        fuelLabel->setSize({columnWidth, 30});
        fuelLabel->getRenderer()->setTextColor(sf::Color::Black);
        fuelLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        fuelLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(fuelLabel, i + 1, 4);

        auto repairButton = tgui::Button::create();
        repairButton->setSize({columnWidth, 25});
        repairButton->setWidgetName(bus.get_id() + "RepairButton");
        repairButton->setText("Repair");
        repairButton->onPress([this, bus] {
            this->_data->states.add_state(Engine::StateRef(new BusMaintenance(this->_data, bus.get_id())), false);
        });
        grid->addWidget(repairButton, i + 1, 5);

        auto improveButton = tgui::Button::create();
        improveButton->setSize({columnWidth, 25});
        improveButton->setWidgetName(bus.get_id() + "ImproveButton");
        improveButton->setText("Improve");
        improveButton->onPress([this, bus] {
            this->_data->states.add_state(Engine::StateRef(new BusImprovementsState(this->_data, bus.get_id())), false);
        });
        grid->addWidget(improveButton, i + 1, 6);
        
    }

    panel->add(grid);
    panel->setWidgetName("Inventory");
    return panel;
}