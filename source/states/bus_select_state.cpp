#include "states/bus_select_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "states/main_menu_state.hpp"
#include <string>

BusSelectState::BusSelectState(GameDataRef data)
    : _data(data), selected_path(-1), bus(Bus()), employee(Employee()), new_simulation_info(&bus, &employee, {})
{
}

void BusSelectState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/bus_select.txt");

    this->_data->gui.get<tgui::Button>("cancel_button")->onPress([this] { this->_data->states.remove_state(); });

    auto send_button_click = [this] {
        bool add = true;

        if (new_simulation_info.bus->get_name() == "" || new_simulation_info.employee->get_name() == "" ||
            new_simulation_info.elements_path.empty())
        {
            auto messageBox = tgui::MessageBox::create();
            messageBox->setTitle("Warning");
            messageBox->setText("You must select an employee,\n a bus and a route");
            messageBox->addButton("OK");
            messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f, this->_data->window->getSize().y / 2 - 50.0f);
            messageBox->setSize(400.0f, 100.f);
            messageBox->onButtonPress([msgBox = messageBox.get()](const tgui::String &button) {
                msgBox->getParent()->remove(msgBox->shared_from_this());
            });
            this->_data->gui.add(messageBox);
            add = false;
        }

        if (add)
        {
            new_simulation_info.bus->set_in_route(true);
            new_simulation_info.employee->set_in_route(true); 
            this->_data->simulation_info.push_back(this->new_simulation_info);
            this->_data->states.remove_state();
        }
    };
    this->_data->gui.get<tgui::Button>("send_button")->onPress(send_button_click);

    this->_data->gui.add(create_selection_panel());
}

void BusSelectState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);

        if (event->is<sf::Event::Closed>())
        {
            this->_data->city.save();
this->_data->window->close();
            break;
        }

        if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>())
        {
            // When the enter key is pressed, switch to the next handler type
            if (keyPress->code == sf::Keyboard::Key::Escape)
            {
                this->_data->city.save();
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
    // Displays rendered objects
    this->_data->window->display();
}

tgui::Panel::Ptr BusSelectState::create_selection_panel()
{
    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    panel->setPosition({10, 90});

    auto info_label = tgui::Label::create();
    info_label->setText("Bus: " + new_simulation_info.bus->get_name() + "\nDriver: " +
                        new_simulation_info.employee->get_name() + "\nRoute: ");
    info_label->setPosition(5.0f, 5.0f);
    info_label->getRenderer()->setTextColor(sf::Color::White);
    panel->add(info_label);

    auto horizontalLayout = tgui::HorizontalLayout::create();
    horizontalLayout->setPosition({5.0f, 50.0f});

    auto busList = tgui::ListBox::create();
    for (const auto &bus : this->_data->player.get_buses())
    {
        if (!bus.get_in_route())
        {
            busList->addItem(bus.get_name(), std::to_string(bus.get_id()));
        }
    }
    busList->onItemSelect([this, info_label](const tgui::String& item, const tgui::String& value) {
        int bus_id = std::stoi(value.toStdString());
        this->new_simulation_info.bus = &this->_data->player.get_bus(bus_id);
        if (!this->_data->routes.empty() && this->selected_path != -1)
        {
            info_label->setText("Bus: " + new_simulation_info.bus->get_name() + "\nDriver: " +
                    new_simulation_info.employee->get_name() + "\nRoute: " + this->_data->routes[selected_path].name);
        }
        else
        {
            info_label->setText("Bus: " + new_simulation_info.bus->get_name() + "\nDriver: " +
                    new_simulation_info.employee->get_name() + "\nRoute: ");
        }
    });
    horizontalLayout->add(busList);

    auto employeeList = tgui::ListBox::create();
    for (const auto &employee : this->_data->player.get_employees())
    {
        if (!employee.get_in_route())
        {
            employeeList->addItem(employee.get_name(), std::to_string(employee.get_id()));
        }  
    }
    employeeList->onItemSelect([this, info_label](const tgui::String& item, const tgui::String& value) {
        int employee_id = std::stoi(value.toStdString());

        if(this->_data->player.get_employee(employee_id).get_total_work_hours() >= 40)
        {
            auto messageBox = tgui::MessageBox::create();
            messageBox->setTitle("Warning");
            messageBox->setText("The selected employee has exceeded 40 work \n hours without payment and cannot be \n assigned to a new route.");
            messageBox->addButton("OK");
            messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f, this->_data->window->getSize().y / 2 - 50.0f);
            messageBox->setSize(400.0f, 100.f);
            messageBox->onButtonPress([msgBox = messageBox.get()](const tgui::String &button) {
                msgBox->getParent()->remove(msgBox->shared_from_this());
            });
            this->_data->gui.add(messageBox);
            return;
        }

        this->new_simulation_info.employee = &this->_data->player.get_employee(employee_id);
        if (!this->_data->routes.empty() && this->selected_path != -1)
        {
            info_label->setText("Bus: " + new_simulation_info.bus->get_name() + "\nDriver: " +
                    new_simulation_info.employee->get_name() + "\nRoute: " + this->_data->routes[selected_path].name);
        }
        else
        {
            info_label->setText("Bus: " + new_simulation_info.bus->get_name() + "\nDriver: " +
                    new_simulation_info.employee->get_name() + "\nRoute: ");
        }
    });
    horizontalLayout->add(employeeList);

    auto pathList = tgui::ListBox::create();
    for (size_t i = 0; i < this->_data->routes.size(); ++i)
    {
        pathList->addItem(this->_data->routes[i].name, std::to_string(i));
    }
    pathList->onItemSelect([this, info_label](int index) {
        if (index != -1)
        {
            this->new_simulation_info.elements_path = this->_data->routes[index].route;
            this->selected_path = index;
            info_label->setText("Bus: " + new_simulation_info.bus->get_name() + "\nDriver: " +
                    new_simulation_info.employee->get_name() + "\nRoute: " + this->_data->routes[selected_path].name);
        }
    });
    horizontalLayout->add(pathList);

    panel->add(horizontalLayout);
    return panel;
}