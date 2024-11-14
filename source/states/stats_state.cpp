#include "states/stats_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

StatsState::StatsState(GameDataRef data) : _data(data), current_info(0)
{
    // empty
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
    comboBox->setPosition(600.0f, 10.0f);
    for(int i = 0; i < this->_data->simulation_info.size(); ++i)
    {
        comboBox->addItem(std::to_string(i));
    }
    comboBox->onItemSelect([this, comboBox] {
        current_info = std::stoi(std::string(comboBox->getSelectedItem()));
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
void StatsState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void StatsState::draw_state(float dt __attribute__((unused)))
{
    this->_data->gui.draw();

    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    panel->setPosition({10, 120});

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    grid->setPosition({5, 5});

    auto busLabel = tgui::Label::create();
    busLabel->setText("Bus: " + this->_data->simulation_info[current_info].bus->get_name());
    busLabel->setPosition({0.0f, 0.0f});
    busLabel->setSize({200.0f, 30.0f});
    busLabel->getRenderer()->setTextColor(sf::Color::White);
    grid->addWidget(busLabel, 0, 0, tgui::Grid::Alignment::Left);

    auto employeeLabel = tgui::Label::create();
    employeeLabel->setText("Employee: " + this->_data->simulation_info[current_info].employee->get_name() + " " + this->_data->simulation_info[current_info].employee->get_last_name());
    employeeLabel->setPosition({0.0f, 30.0f});
    employeeLabel->setSize({200.0f, 30.0f});
    employeeLabel->getRenderer()->setTextColor(sf::Color::White);
    grid->addWidget(employeeLabel, 1, 0, tgui::Grid::Alignment::Left);

    auto timesListView = tgui::ListView::create();
    timesListView->addColumn("Track", 150);
    timesListView->addColumn("Time", 100);
    for(int i = 0; i < this->_data->simulation_info[current_info].track_times.size(); ++i)
    {
        timesListView->addItem({this->_data->simulation_info[current_info].track_names[i], std::to_string(this->_data->simulation_info[current_info].track_times[i])});
    }

    auto passengersListView = tgui::ListView::create();
    passengersListView->addColumn("Bus Stop", 150);
    passengersListView->addColumn("On", 50);
    passengersListView->addColumn("Off", 50);
    for(int i = 0; i < this->_data->simulation_info[current_info].passenger_stop_names.size(); ++i)
    {
        passengersListView->addItem({
            this->_data->simulation_info[current_info].passenger_stop_names[i], 
            std::to_string(this->_data->simulation_info[current_info].passengers_per_stop[i].first), 
            std::to_string(this->_data->simulation_info[current_info].passengers_per_stop[i].second)
        });
    }

    auto horizontalLayout = tgui::HorizontalLayout::create();
    horizontalLayout->add(timesListView);
    horizontalLayout->add(passengersListView);
    horizontalLayout->setPosition({0.0f, 60.0f});
    horizontalLayout->setSize({this->_data->window->getSize().x - 20.0f, 340.0f});


    grid->addWidget(horizontalLayout, 2, 0);

    panel->add(grid);
    this->_data->gui.add(panel);

    // Displays rendered objects
    this->_data->window->display();
}
