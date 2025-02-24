#include "states/route_list_state.hpp"
#include "states/route_edit_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include "utils/JSON.hpp"

RouteListState::RouteListState(GameDataRef data) : _data(data)
{
    // empty
}

void RouteListState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Routes");
    
    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.remove_state();
    });

    auto newRouteButton = tgui::Button::create();
    newRouteButton->setSize({120, 25});
    newRouteButton->setPosition({600, 90});
    newRouteButton->setText("New Route");
    newRouteButton->onPress([this] { 
        this->_data->routes.push_back(Route());
        this->_data->states.add_state(Engine::StateRef(new RouteEditState(this->_data, this->_data->routes.back())), true);
    });
    this->_data->gui.add(newRouteButton);
    this->_data->gui.add(create_route_list());
}

void RouteListState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);

        if (event->is<sf::Event::Closed>())
        {
            util::save_route_vector(this->_data->routes);
            this->_data->window->close();
            break;
        }

        if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>())
        {
            // When the enter key is pressed, switch to the next handler type
            if (keyPress->code == sf::Keyboard::Key::Escape)
            {
                util::save_route_vector(this->_data->routes);
                this->_data->window->close();
                break;
            }
        }
    }
}

// marks dt to not warn compiler
void RouteListState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void RouteListState::draw_state(float dt __attribute__((unused)))
{
    this->_data->gui.draw();
    
    // Displays rendered objects
    this->_data->window->display();
}

tgui::ScrollablePanel::Ptr RouteListState::create_route_list()
{
    auto panel = tgui::ScrollablePanel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    panel->setPosition({10, 120});

    auto grid = tgui::Grid::create();
    grid->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    grid->setPosition({5.0f, 5.0f});
    grid->setAutoSize(true);

    // Add headers
    auto nameLabel = tgui::RichTextLabel::create("Route Name");
    nameLabel->setSize({200, 30});
    nameLabel->getRenderer()->setTextColor(sf::Color::White);
    nameLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    nameLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    nameLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(nameLabel, 0, 0);

    // auto priceLabel = tgui::RichTextLabel::create("Length");
    // priceLabel->setSize({120, 30});
    // priceLabel->getRenderer()->setTextColor(sf::Color::White);
    // priceLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    // priceLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    // priceLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    // grid->addWidget(priceLabel, 0, 1);

    auto amountLabel = tgui::RichTextLabel::create("Number of stops");
    amountLabel->setSize({120, 30});
    amountLabel->getRenderer()->setTextColor(sf::Color::White);
    amountLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    amountLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    amountLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
    grid->addWidget(amountLabel, 0, 2);

    // Add routes to the grid
    for (int i = 0; i < this->_data->routes.size(); ++i)
    {
        Route &route = this->_data->routes[i];

        auto routeName = tgui::RichTextLabel::create(route.name);
        routeName->setSize({120, 30});
        routeName->getRenderer()->setTextColor(sf::Color::White);
        routeName->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        routeName->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(routeName, i+1, 0);

        auto busStopsNum = 0;
        for (auto visual_element : route.route)
        {
            auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element);
            if (bus_stop)
            {
                busStopsNum++;
            }
        }

        auto routeNumberStops = tgui::RichTextLabel::create(std::to_string(busStopsNum));
        routeNumberStops->setSize({120, 30});
        routeNumberStops->getRenderer()->setTextColor(sf::Color::White);
        routeNumberStops->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        routeNumberStops->setVerticalAlignment(tgui::VerticalAlignment::Center);
        grid->addWidget(routeNumberStops, i+1, 2);

        auto editButton = tgui::Button::create();
        editButton->setSize({120, 25});
        editButton->setText("Edit");
        editButton->onPress([this, &route] { 
            this->_data->states.add_state(Engine::StateRef(new RouteEditState(this->_data, route)), true);
        });
        grid->addWidget(editButton, i+1, 3);

        auto deleteButton = tgui::Button::create();
        deleteButton->setSize({120, 25});
        deleteButton->setText("Delete");
        deleteButton->onPress([this, i] { 
            this->_data->routes.erase(this->_data->routes.begin() + i);
            init_state(); 
        });
        grid->addWidget(deleteButton, i+1, 4);
    }

    panel->add(grid);
    panel->setWidgetName("StorePanel");
    return panel;
}