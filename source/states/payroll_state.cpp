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
    this->_data->gui.loadWidgetsFromFile("assets/screens/payroll.txt");

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
    headerName.setString("Name");
    headerName.setCharacterSize(20);
    headerName.setFillColor(sf::Color::Black);
    headerName.setStyle(sf::Text::Bold);
    headerName.setPosition({10.0f, 90.0f});
    this->_data->window->draw(headerName);

    sf::Text headerLastName(font);
    headerLastName.setString("Last Name");
    headerLastName.setCharacterSize(20);
    headerLastName.setFillColor(sf::Color::Black);
    headerLastName.setStyle(sf::Text::Bold);
    headerLastName.setPosition({130.0f, 90.0f});
    this->_data->window->draw(headerLastName);

    sf::Text headerWorkTime(font);
    headerWorkTime.setString("Accumulated \n work time");
    headerWorkTime.setCharacterSize(20);
    headerWorkTime.setFillColor(sf::Color::Black);
    headerWorkTime.setStyle(sf::Text::Bold);
    headerWorkTime.setPosition({280.0f, 90.0f});
    this->_data->window->draw(headerWorkTime);

    sf::Text headerHourlyRate(font);
    headerHourlyRate.setString("Hourly Rate");
    headerHourlyRate.setCharacterSize(20);
    headerHourlyRate.setFillColor(sf::Color::Black);
    headerHourlyRate.setStyle(sf::Text::Bold);
    headerHourlyRate.setPosition({430.0f, 90.0f});
    this->_data->window->draw(headerHourlyRate);

    sf::Text headerPaycheck(font);
    headerPaycheck.setString("Paycheck");
    headerPaycheck.setCharacterSize(20);
    headerPaycheck.setFillColor(sf::Color::Black);
    headerPaycheck.setStyle(sf::Text::Bold);
    headerPaycheck.setPosition({580.0f, 90.0f});
    this->_data->window->draw(headerPaycheck);

    auto employees = this->_data->player.get_employees();
    for (int i = 0; i < employees.size(); ++i)
    {
        const auto &employee = employees[i];

        // Create a text object for the employee name
        sf::Text employeeNameText(font);
        employeeNameText.setString(employee.get_name());
        employeeNameText.setCharacterSize(20);
        employeeNameText.setFillColor(sf::Color::Black);
        employeeNameText.setStyle(sf::Text::Regular);
        employeeNameText.setPosition({10.0f, 135.0f + (i * 30.0f)});
        this->_data->window->draw(employeeNameText);

        // Create a text object for the employee last name
        sf::Text employeeLastNameText(font);
        employeeLastNameText.setString(employee.get_last_name());
        employeeLastNameText.setCharacterSize(20);
        employeeLastNameText.setFillColor(sf::Color::Black);
        employeeLastNameText.setStyle(sf::Text::Regular);
        employeeLastNameText.setPosition({130.0f, 135.0f + (i * 30.0f)});
        this->_data->window->draw(employeeLastNameText);

        // Create a text object for the accumulated work time
        sf::Text workTimeText(font);
        workTimeText.setString(std::to_string(employee.get_accumulated_work_time()));
        workTimeText.setCharacterSize(20);
        workTimeText.setFillColor(sf::Color::Black);
        workTimeText.setStyle(sf::Text::Regular);
        workTimeText.setPosition({280.0f, 135.0f + (i * 30.0f)});
        this->_data->window->draw(workTimeText);

        // Create a text object for the hourly rate
        sf::Text hourlyRateText(font);
        hourlyRateText.setString(std::to_string(employee.get_hourly_rate()));
        hourlyRateText.setCharacterSize(20);
        hourlyRateText.setFillColor(sf::Color::Black);
        hourlyRateText.setStyle(sf::Text::Regular);
        hourlyRateText.setPosition({430.0f, 135.0f + (i * 30.0f)});
        this->_data->window->draw(hourlyRateText);

        // Create a text object for the paycheck
        sf::Text paycheckText(font);
        paycheckText.setString(std::to_string(employee.get_hourly_rate() * employee.get_accumulated_work_time()));
        paycheckText.setCharacterSize(20);
        paycheckText.setFillColor(sf::Color::Black);
        paycheckText.setStyle(sf::Text::Regular);
        paycheckText.setPosition({580.0f, 135.0f + (i * 30.0f)});
        this->_data->window->draw(paycheckText);
    }

    // Displays rendered objects
    this->_data->window->display();
}
