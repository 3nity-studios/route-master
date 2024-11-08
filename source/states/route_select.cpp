#include "states/route_select.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "engine/input_manager.hpp"
#include "states/bus_select_state.hpp"
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <string>

RouteSelect::RouteSelect(GameDataRef data)
    : _data(data), map_icons_texture(sf::Image(sf::Vector2u(25, 25), sf::Color::Green)), sprite_pressed(false)
{
}

void RouteSelect::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/route_select.txt");

    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 370.0f});
    panel->setPosition({10, 90});
    this->_data->gui.add(panel);

    auto nameLabel = tgui::Label::create();
    nameLabel->setText("Route Name:");
    nameLabel->setPosition({5.0f, 5.0f});
    nameLabel->setSize({150.0f, 30.0f});
    nameLabel->getRenderer()->setTextColor(sf::Color::Black);
    panel->add(nameLabel);

    auto nameEditBox = tgui::EditBox::create();
    nameEditBox->setSize({200.0f, 30.0f});
    nameEditBox->setPosition({150.0f, 5.0f});
    nameEditBox->onTextChange([this](const tgui::String& text){
        this->route.name = text.toStdString();
    });
    panel->add(nameEditBox);

    this->canvas = tgui::CanvasSFML::create({800, 400});
    this->canvas->setPosition({5.0f, 40.0f});
    panel->add(this->canvas);

    this->_data->gui.get<tgui::Button>("cancel_button")->onPress([this] { this->_data->states.remove_state(); });

    this->_data->gui.get<tgui::Button>("select_button")->onPress([this] {
        if (this->route.route.empty())
        {
            auto messageBox = tgui::MessageBox::create();
            messageBox->setTitle("Warning");
            messageBox->setText("The path is empty");
            messageBox->addButton("OK");
            messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f,
                                    this->_data->window->getSize().y / 2 - 50.0f);
            messageBox->setSize(400.0f, 100.f);
            messageBox->onButtonPress([msgBox = messageBox.get()](const tgui::String &button) {
                msgBox->getParent()->remove(msgBox->shared_from_this());
            });
            this->_data->gui.add(messageBox);
        }
        else
        {
            this->_data->routes.push_back(this->route);
            this->_data->states.remove_state();
        }
    });

    if (!map_icons_texture.loadFromFile("assets/img/map_icons.png"))
    {
        throw GameException("Couldn't find file: assets/img/map_icons.png");
    }

    sf::View view(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    this->canvas->setView(view);

    init_visual_elements();
}

/// the last known mouse position
sf::Vector2i previous_mouse_pos;
/// whether we are dragging or not
bool is_dragging = false;

void RouteSelect::update_inputs()
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

        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if ((keyPress->button == sf::Mouse::Button::Left) && !sprite_pressed)
            {
                for (auto &element : visual_elements)
                {
                    if (this->_data->inputs.is_sprite_clicked(element.first, sf::Mouse::Button::Left, *this->_data->window, this->canvas->getRenderTexture()))
                    {
                        // std::cout << "Detected a click on sprite at " << sf::Mouse::getPosition(*this->_data->window).x  << ", " << sf::Mouse::getPosition(*this->_data->window).y << std::endl;
                        if (add_to_path(element.second))
                        {
                            element.first.setColor(sf::Color(255, 255, 255));
                        }

                        break;
                    }
                }

                sprite_pressed = true;

                return;
            }
            // if mouse button is pressed start dragging
            if (keyPress->button == sf::Mouse::Button::Right)
            {
                is_dragging = true;
                return;
            }
        }

        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (sprite_pressed)
            {
                sprite_pressed = false;
            }
            // if mouse button is released stop draggin
            if (keyPress->button == sf::Mouse::Button::Right)
            {
                is_dragging = false;
                return;
            }
        }

        if (const auto *keyPress = event->getIf<sf::Event::KeyPressed>())
        {
            // When the enter key is pressed, switch to the next handler type
            if (keyPress->code == sf::Keyboard::Key::Escape)
            {
                this->_data->states.remove_state();
                break;
            }
        }

        // if dragging mouse
        if (const auto *keyPress = event->getIf<sf::Event::MouseMoved>())
        {
            // get mouse position
            const sf::Vector2i mouse_position{keyPress->position.x, keyPress->position.y};
            // if dragging, move view
            if (is_dragging)
            {
                sf::RenderTarget &target{this->canvas->getRenderTexture()};

                // calculate how far mouse has moved in view
                const auto delta = target.mapPixelToCoords(mouse_position) - target.mapPixelToCoords(previous_mouse_pos);
                // apply negatively to view
                auto view = target.getView();
                view.move(-delta);
                target.setView(view);
            }
            // update previous mouse position
            previous_mouse_pos = mouse_position;
            return;
        }
    }
}

// marks dt to not warn compiler
void RouteSelect::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void RouteSelect::draw_state(float dt __attribute__((unused)))
{
    // background color
    this->_data->gui.draw();

    this->canvas->clear(sf::Color::White);

    draw_lines();

    for (auto visual_element : visual_elements)
    {
        this->canvas->draw(visual_element.first);
        // std::cout << "Created sprite at " << visual_element.first.getPosition().x << ", " << visual_element.first.getPosition().y << std::endl;
    }

    // Displays rendered objects
    this->_data->window->display();
    this->canvas->display();
}

void RouteSelect::init_visual_elements()
{
    sf::IntRect traffic_light_rect(sf::Vector2i(0, 0), sf::Vector2i(27, 53));
    sf::IntRect bus_stop_rect(sf::Vector2i(44, 10), sf::Vector2i(52, 52));
    sf::IntRect curve_rect(sf::Vector2i(98, 11), sf::Vector2i(53, 52));

    for (auto visual_element : this->_data->city.get_visual_elements())
    {
        auto visual_element_pointer = visual_element->get_info();
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element_pointer);
        auto traffic_light = std::dynamic_pointer_cast<TrafficLight>(visual_element_pointer);

        if (bus_stop)
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setPosition(sf::Vector2f(bus_stop->get_x(), bus_stop->get_y()));
            sprite.setTextureRect(bus_stop_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_pair<sf::Sprite, std::shared_ptr<VisualElement>>(
                std::move(sprite), std::move(visual_element_pointer)));
        }
        else if (traffic_light)
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setPosition(sf::Vector2f(traffic_light->get_x(), traffic_light->get_y()));
            sprite.setTextureRect(traffic_light_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_pair<sf::Sprite, std::shared_ptr<VisualElement>>(
                std::move(sprite), std::move(visual_element_pointer)));
        }
        else
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setPosition(sf::Vector2f(visual_element->get_info()->get_x(), visual_element->get_info()->get_y()));
            sprite.setTextureRect(curve_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_pair<sf::Sprite, std::shared_ptr<VisualElement>>(
                std::move(sprite), std::move(visual_element_pointer)));
        }
    }
}

float calc_distance1(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

void RouteSelect::draw_lines()
{
    for (auto element : this->_data->city.get_streets())
    {
        sf::Vertex ver1;
        ver1.position =
            sf::Vector2f(element->get_src_node()->get_info()->get_x(), element->get_src_node()->get_info()->get_y());
        ver1.color = sf::Color::Black;
        sf::Vertex ver2;
        ver2.position =
            sf::Vector2f(element->get_tgt_node()->get_info()->get_x(), element->get_tgt_node()->get_info()->get_y());
        ver2.color = sf::Color::Black;

        sf::Vertex line[] = {ver1, ver2};

        this->canvas->draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (int i = 1; i < route.route.size(); i++)
    {
        auto element = route.route.at(i);
        auto element2 = route.route.at(i - 1);

        sf::Vertex ver1;
        ver1.position = sf::Vector2f(element->get_x(), element->get_y());
        ver1.color = sf::Color::Yellow;
        sf::Vertex ver2;
        ver2.position = sf::Vector2f(element2->get_x(), element2->get_y());
        ver2.color = sf::Color::Yellow;

        sf::Vertex line[] = {ver1, ver2};

        this->canvas->draw(line, 2, sf::PrimitiveType::Lines);
    };
}

bool RouteSelect::add_to_path(std::shared_ptr<VisualElement> visual_element)
{
    if (route.route.empty())
    {
        route.route.push_back(visual_element);
        return true;
    }

    bool connected = false;

    for (auto element : this->_data->city.get_streets())
    {
        if (element->get_tgt_node()->get_info()->get_id() == visual_element->get_id() &&
            element->get_src_node()->get_info()->get_id() == route.route.back()->get_id())
        {
            connected = true;
        }
    }

    if (connected)
    {
        route.route.push_back(visual_element);
    }
    else
    {
        auto messageBox = tgui::MessageBox::create();
        messageBox->setTitle("Warning");
        messageBox->setText("The elements are not connected");
        messageBox->addButton("OK");
        messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f,
                                this->_data->window->getSize().y / 2 - 50.0f);
        messageBox->setSize(400.0f, 100.f);
        messageBox->onButtonPress([msgBox = messageBox.get()](const tgui::String &button) {
            msgBox->getParent()->remove(msgBox->shared_from_this());
        });
        this->_data->gui.add(messageBox);
    }

    return connected;
}

RouteSelect::~RouteSelect()
{
}
