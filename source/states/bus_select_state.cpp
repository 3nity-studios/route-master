#include "states/bus_select_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

BusSelectState::BusSelectState(GameDataRef data) : _data(data), new_simulation_info(SimulationInfo(Bus(), Employee(), {})), selected_path(0)
{
}

void BusSelectState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    buttons_gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/bus_select.txt");
    sf::FloatRect rect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1000, 600));
    this->_data->window->setView(sf::View(rect));

    this->_data->gui.get<tgui::Button>("cancel_button")->onPress([this] {
        this->_data->states.remove_state();
    });

    auto send_button_click = [this]{
        bool add = true; 

        if (new_simulation_info.bus.get_name() == "" || new_simulation_info.employee.get_name() == "" || new_simulation_info.elements_path.empty())
        {
            advertisement = "You must select all parameters";
            add = false; 
        }

        if (add)
        {
            this->_data->simulation_info.push_back(this->new_simulation_info);
            this->_data->states.remove_state();
        }
    };

    this->_data->gui.get<tgui::Button>("send_button")->onPress(send_button_click);

    update_buttons();
}

void BusSelectState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);
        buttons_gui.handleEvent(*event);

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
void BusSelectState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void BusSelectState::draw_state(float dt __attribute__((unused)))
{
    this->_data->gui.draw();
    buttons_gui.draw();

    // write text
    sf::Font font("assets/fonts/joystix.ttf");

    // throws error if can't load font
    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        // error...
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    draw_table(font);

    sf::Text principal_text(font);
    principal_text.setString("Bus: " + new_simulation_info.bus.get_name() + "Driver: " + new_simulation_info.employee.get_name() + "Path: " + std::to_string(selected_path));
    principal_text.setPosition(sf::Vector2f(110.f, 450.f));
    principal_text.setCharacterSize(24);
    principal_text.setFillColor(sf::Color::Black);

    sf::Text advertisement_text(font);
    advertisement_text.setString(advertisement);
    advertisement_text.setPosition(sf::Vector2f(700.f, 450.f));
    advertisement_text.setCharacterSize(20);
    advertisement_text.setFillColor(sf::Color::Black);

    this->_data->window->draw(advertisement_text);
    this->_data->window->draw(principal_text);

    // Displays rendered objects
    this->_data->window->display();
}

void BusSelectState::update_buttons()
{
    buttons_gui.removeAllWidgets();

    int i = 0;
    for (const auto &bus : this->_data->player.get_buses())
    {
        // Create a button for select a bus
        auto selectButton = tgui::Button::create();
        selectButton->setWidgetName(bus.get_id() + "SelectBus");
        selectButton->setPosition({100.f, 120.0f + (i * 30.0f)});
        selectButton->setText("Select " + bus.get_name());
        selectButton->onPress([this, bus, i] { 
            this->new_simulation_info.bus = this->_data->player.get_buses().select(i);
        });
        buttons_gui.add(selectButton);
        ++i;
    }

    i = 0;
    for (const auto &employee : this->_data->player.get_employees())
    {
        // Create a button for select a employee
        auto selectButton = tgui::Button::create();
        selectButton->setWidgetName(employee.get_id() + "SelectEmployee");
        selectButton->setPosition({360.0f, 120.0f + (i * 30.0f)});
        selectButton->setText("Select " + employee.get_name());
        selectButton->onPress([this, employee, i] { 
            this->new_simulation_info.employee = this->_data->player.get_employees().select(i);
        });
        buttons_gui.add(selectButton);
        ++i;
    }

    i = 0;
    for (const auto &path : this->_data->paths)
    {
        // Create a button for select a path
        auto selectButton = tgui::Button::create();
        selectButton->setWidgetName(i + "SelectEmployee");
        selectButton->setPosition({650.0f, 120.0f + (i * 30.0f)});
        selectButton->setText("Select path " + std::to_string(i + 1));
        selectButton->onPress([this, path, i] { 
            this->new_simulation_info.elements_path = this->_data->paths.at(i);
            this->selected_path = i+1;
        });
        buttons_gui.add(selectButton);
        ++i;
    }
}

void BusSelectState::draw_table(sf::Font font)
{
    sf::RectangleShape rectangle(sf::Vector2f(110, 80));
    rectangle.setPosition(sf::Vector2f(10.f, 80.f));
    rectangle.setFillColor(sf::Color(255, 255, 255, 128));
    rectangle.setSize(sf::Vector2f(115, 370));
    this->_data->window->draw(rectangle);

    sf::RectangleShape rectangle1(sf::Vector2f(110, 80));
    rectangle1.setPosition(sf::Vector2f(300.f, 80.f));
    rectangle1.setFillColor(sf::Color(255, 255, 255, 128));
    rectangle1.setSize(sf::Vector2f(150, 370));
    this->_data->window->draw(rectangle1);

    sf::RectangleShape rectangle2(sf::Vector2f(110, 80));
    rectangle2.setPosition(sf::Vector2f(680.f, 80.f));
    rectangle2.setFillColor(sf::Color(255, 255, 255, 128));
    rectangle2.setSize(sf::Vector2f(130, 370));
    this->_data->window->draw(rectangle2);

    // Draw table headers
    sf::Text headerBuses(font);
    headerBuses.setString("Buses");
    headerBuses.setCharacterSize(20);
    headerBuses.setFillColor(sf::Color::Black);
    headerBuses.setStyle(sf::Text::Bold);
    headerBuses.setPosition({15.0f, 90.0f});
    this->_data->window->draw(headerBuses);

    sf::Text headerEmployees(font);
    headerEmployees.setString("Employees");
    headerEmployees.setCharacterSize(20);
    headerEmployees.setFillColor(sf::Color::Black);
    headerEmployees.setStyle(sf::Text::Bold);
    headerEmployees.setPosition({305.0f, 90.0f});
    this->_data->window->draw(headerEmployees);

    sf::Text headerPath(font);
    headerPath.setString("Paths");
    headerPath.setCharacterSize(20);
    headerPath.setFillColor(sf::Color::Black);
    headerPath.setStyle(sf::Text::Bold);
    headerPath.setPosition({690.0f, 90.0f});
    this->_data->window->draw(headerPath);


    int i = 0; 

    for (auto bus : this->_data->player.get_buses())
    {
        sf::Text text(font);
        text.setString(bus.get_name());
        text.setCharacterSize(24);
        text.setPosition(sf::Vector2f(20.f, 120.0f + (i * 30.0f)));
        text.setFillColor(sf::Color::Black);

        i++;

        this->_data->window->draw(text);
    }

    i = 0; 

    for (auto employee : this->_data->player.get_employees())
    {
        sf::Text text(font);
        text.setString(employee.get_name());
        text.setCharacterSize(24);
        text.setPosition(sf::Vector2f(320.f, 120.0f + (i * 30.0f)));
        text.setFillColor(sf::Color::Black);

        i++;

        this->_data->window->draw(text);
    }

    i = 0; 

    for (auto path : this->_data->paths)
    {
        sf::Text text(font);
        text.setString("Path " + std::to_string(i + 1));
        text.setCharacterSize(24);
        text.setPosition(sf::Vector2f(690.f, 120.0f + (i * 30.0f)));
        text.setFillColor(sf::Color::Black);

        i++;

        this->_data->window->draw(text);
    }
}