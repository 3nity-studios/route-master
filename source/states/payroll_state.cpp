#include "states/payroll_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

PayrollState::PayrollState(GameDataRef data) : _data(data)
{
    
}

void PayrollState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Payroll");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
    });

    auto employees = this->_data->player.get_employees();
    for (int i = 0; i < employees.size(); ++i)
    {
        auto &employee = employees[i];

        std::cout << employee.get_name();
        
        // Create a button for doing maintenance to the bus
        auto buyButton = tgui::Button::create();
        buyButton->setWidgetName(employee.get_id() + "PayButton");
        buyButton->setPosition({700.0f, 135.0f + (i * 30.0f)});
        buyButton->setText("Pay " + employee.get_name());
        buyButton->onPress([this, employee] {
            this->_data->store.pay_employee(employee.get_id(), this->_data->player);
        });
        this->_data->gui.add(buyButton);
    }
}

void PayrollState::update_inputs()
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
void PayrollState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void PayrollState::draw_state(float dt __attribute__((unused)))
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

    int columnWidth = 120;
    auto listView = tgui::ListView::create();
    listView->addColumn("Name", columnWidth, tgui::HorizontalAlignment::Center);
    listView->addColumn("Last Name", columnWidth, tgui::HorizontalAlignment::Center);
    listView->addColumn("Accumulated\nWork Time", columnWidth, tgui::HorizontalAlignment::Center);
    listView->addColumn("Hourly Rate", columnWidth, tgui::HorizontalAlignment::Center);
    listView->addColumn("Paycheck", columnWidth, tgui::HorizontalAlignment::Center);
    listView->setSize({690.0f, 400.0f});
    listView->setPosition({10.0f, 90.0f});
    listView->setResizableColumns(true);
    listView->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    listView->getRenderer()->setTextSize(12);

    auto employees = this->_data->player.get_employees();
    for (const auto& employee : employees)
    {
        listView->addItem({
            employee.get_name(),
            employee.get_last_name(),
            std::to_string(employee.get_accumulated_work_time()),
            std::to_string(employee.get_hourly_rate()),
            std::to_string(employee.get_hourly_rate() * employee.get_accumulated_work_time())
        });
    }

    this->_data->gui.add(listView);

    // Displays rendered objects
    this->_data->window->display();
}
