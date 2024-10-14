#include "states/store_state.hpp"
#include "states/main_menu_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

StoreState::StoreState(GameDataRef data) : _data(data)
{
    buses.append(Bus(1, "Bus1", 50, {}, 5));
    buses.append(Bus(2, "Bus2", 60, {}, 10));
    buses.append(Bus(3, "Bus3", 55, {}, 7));
    buses.append(Bus(4, "Bus4", 45, {}, 3));
    buses.append(Bus(5, "Bus5", 70, {}, 12));

    employees.append(Employee(1, "John", "Doe", 25, 5, 0));
    employees.append(Employee(2, "Jane", "Smith", 30, 7, 1));
    employees.append(Employee(3, "Alice", "Johnson", 28, 6, 2));
    employees.append(Employee(4, "Bob", "Brown", 35, 10, 3));
    employees.append(Employee(5, "Charlie", "Davis", 40, 15, 4));
}

void StoreState::init_state()
{
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/store.txt");
}

void StoreState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);
        this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
            this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);
        });

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
    // END SAMPLE RENDER CODE
    this->_data->gui.draw();

    // write text
    sf::Font font("assets/fonts/joystix.ttf");

    // throws error if can't load font
    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        // error...
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    for (const auto& bus : buses)
    {
        // Create a text object for the bus name
        sf::Text busText(font);
        busText.setString(bus.get_name());
        busText.setCharacterSize(20);
        busText.setFillColor(sf::Color::Black);
        busText.setStyle(sf::Text::Regular);

        // Set the position of the text
        busText.setPosition({10.0f, 90.0f + (bus.get_id() * 30.0f)});
        this->_data->window->draw(busText);

        // Create a button for buying the bus
        auto buyButton = tgui::Button::create();
        buyButton->setPosition({200.0f, 90.0f + (bus.get_id() * 30.0f)});
        buyButton->setText("Buy " + bus.get_name());
        buyButton->onPress([this, bus] {
            // Handle the bus purchase logic here
            // this->_data->player.buy(bus);
        });
        this->_data->gui.add(buyButton);
    }

    for (const auto& employee : employees)
    {
        // Create a text object for the employee name
        sf::Text employeeText(font);
        employeeText.setString(employee.get_name() + " " + employee.get_last_name());
        employeeText.setCharacterSize(20);
        employeeText.setFillColor(sf::Color::Black);
        employeeText.setStyle(sf::Text::Regular);

        // Set the position of the text
        employeeText.setPosition({280.0f, 90.0f + (employee.get_id() * 30.0f)});
        this->_data->window->draw(employeeText);

        // Create a button for hiring the employee
        auto hireButton = tgui::Button::create();
        hireButton->setPosition({420.0f, 90.0f + (employee.get_id() * 30.0f)});
        hireButton->setText("Hire " + employee.get_name());
        hireButton->onPress([this, employee] {
            // Handle the employee hiring logic here
            // this->_data->player.hire(employee);
        });
        this->_data->gui.add(hireButton);
    }

    // Displays rendered objects
    this->_data->window->display();
}
