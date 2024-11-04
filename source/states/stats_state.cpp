#include "states/stats_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

StatsState::StatsState(GameDataRef data) : _data(data)
{
    BusStop stop1(1, "Stop1", {2, 3, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 750.f, 5.f);
    BusStop stop2(2, "Stop2", {2, 3, 4}, 3.0, 10.0, 3.0, 3.0, 2.0, 500.f, 5.f);
    BusStop stop3(3, "Stop3", {2, 3, 3}, 3.0, 5.0, 3.0, 3.0, 2.0, 350.f, 5.f);
    BusStop stop4(4, "Stop4", {2, 4, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 350.f, 250.f);
    BusStop stop5(5, "Stop5", {2, 5, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 600.f, 250.f);
    BusStop stop6(6, "Stop6", {2, 6, 3}, 3.0, 15.0, 3.0, 3.0, 2.0, 600.f, 500.f);

    TrafficLight light1(7, std::vector<std::pair<StreetConnectionIDs, bool>>{std::make_pair<StreetConnectionIDs, bool>(std::make_pair<int, int>(6,7), true)}, 10, 450.f, 250.f);

    VisualElement curve1(8, 220.f, 50.f);
    VisualElement curve2(9, 220.f, 250.f);
    VisualElement curve3(10, 745.f, 275.f);
    VisualElement curve4(11, 745.f, 500.f);

    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);
    city.add_bus_stop(stop4);
    city.add_traffic_light(light1);
    city.add_bus_stop(stop5);
    city.add_bus_stop(stop6);
    city.add_curve(curve1);
    city.add_curve(curve2);
    city.add_curve(curve3);
    city.add_curve(curve4);

    city.initialize_bus_stops();

    Street street1(1, "Street1", 10, 200.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", 10, 50.0f, 2.0f, 0.1f, 0.05f);
    Street street3(3, "Street3", 10, 100.0f, 2.0f, 0.1f, 0.05f);
    Street street4(4, "Street4", 10, 25.0f, 2.0f, 0.1f, 0.05f);
    Street street5(5, "Street5", 10, 50.0f, 2.0f, 0.1f, 0.05f);
    Street street6(6, "Street6", 10, 50.0f, 2.0f, 0.1f, 0.05f);
    Street street7(7, "Street7", 10, 50.0f, 2.0f, 0.1f, 0.05f);
    Street street8(8, "Street8", 10, 100.0f, 2.0f, 0.1f, 0.05f);
    Street street9(9, "Street9", 10, 50.0f, 2.0f, 0.1f, 0.05f);
    Street street10(10, "Street10", 10, 50.0f, 2.0f, 0.1f, 0.05f);

    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    city.add_street(street3, 3, 8);
    city.add_street(street4, 8, 9);
    city.add_street(street5, 9, 4);
    city.add_street(street6, 4, 7);
    city.add_street(street7, 7, 5);
    city.add_street(street8, 5, 10);
    city.add_street(street9, 10, 11);
    city.add_street(street10, 11, 6);

    StreetArcList path;

    for (int i = 1; i < 6; i++)
    {
        for (auto street : city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path.append(street);
            }
        }
    }

    StreetArcList path2;

    for (int i = 6; i < 11; i++)
    {
        for (auto street : city.get_streets())
        {
            if (street->get_info().get_id() == i)
            {
                path2.append(street);
            }
        }
    }

    Bus bus_sim(1, "Bus 1", 10, {}, 5);
    Employee driver_sim(1, "John", "Doe", 25, 50, 5, 0);

    Bus bus_sim2(2, "Bus 2", 15, {}, 5);
    Employee driver_sim2(2, "Rodrigo", "Hernandez", 23, 30, 12, 0);

    SimulationInfo simulation(bus_sim, driver_sim, path);

    this->infos.push_back(simulation);

    city.update();
    city.update_passengers();
}

void StatsState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Statistics");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.remove_state();
    });

    tgui::ComboBox::Ptr comboBox = tgui::ComboBox::create();
    comboBox->setWidgetName("filter_combobox");
    comboBox->setPosition(600.0f, 90.0f);
    comboBox->addItem("All");
    comboBox->addItem("Buses");
    comboBox->addItem("Employees");
    comboBox->onItemSelect([this, comboBox] {
    });
    comboBox->setSelectedItem("All");
    this->_data->gui.add(comboBox);
}

void StatsState::update_inputs()
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
void StatsState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void StatsState::draw_state(float dt __attribute__((unused)))
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

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({10, 90});

    auto busLabel = tgui::Label::create();
    busLabel->setText("Bus: " + infos[0].bus.get_name());
    busLabel->setPosition({0.0f, 0.0f});
    busLabel->setSize({200.0f, 30.0f});
    grid->addWidget(busLabel, 0, 0, tgui::Grid::Alignment::Left);

    auto employeeLabel = tgui::Label::create();
    employeeLabel->setText("Employee: " + infos[0].employee.get_name() + " " + infos[0].employee.get_last_name());
    employeeLabel->setPosition({0.0f, 30.0f});
    employeeLabel->setSize({200.0f, 30.0f});
    grid->addWidget(employeeLabel, 1, 0, tgui::Grid::Alignment::Left);

    auto timesListView = tgui::ListView::create();
    timesListView->addColumn("Element");
    timesListView->addColumn("Time");
    for(int i = 0; i < infos[0].times.size(); ++i)
    {
        timesListView->addItem({"", std::to_string(infos[0].times[i])});
    }

    auto passengersListView = tgui::ListView::create();
    passengersListView->addColumn("Bus Stop");
    passengersListView->addColumn("On");
    passengersListView->addColumn("Off");
    for(int i = 0; i < infos[0].times.size(); ++i)
    {
        passengersListView->addItem({"", std::to_string(infos[0].passengers_per_stop[i].first) + ", " + std::to_string(infos[0].passengers_per_stop[i].second)});
    }

    auto horizontalLayout = tgui::HorizontalLayout::create();
    horizontalLayout->add(timesListView);
    horizontalLayout->add(passengersListView);
    horizontalLayout->setPosition({0.0f, 60.0f});
    horizontalLayout->setSize({this->_data->window->getSize().x - 20.0f, 340.0f});


    grid->addWidget(horizontalLayout, 2, 0);

    this->_data->gui.add(grid);

    // Displays rendered objects
    this->_data->window->display();
}
