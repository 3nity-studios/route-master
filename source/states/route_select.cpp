#include "states/route_select.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include <cmath>
#include "states/bus_select_state.hpp"

RouteSelect::RouteSelect(GameDataRef data) : _data(data), bus_stop_texture(sf::Image(sf::Vector2u(25, 25), sf::Color::Green)), curve_texture (sf::Image(sf::Vector2u(25, 25), sf::Color::Red)), traffic_light_texture(sf::Image(sf::Vector2u(25, 25), sf::Color::Blue)), sprite_pressed(false)
{
}

void RouteSelect::init_state()
{ 
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->gui.setWindow(*this->_data->window);
    this->gui.loadWidgetsFromFile("assets/screens/route_select.txt");

    this->gui.get<tgui::Button>("cancel_button")->onPress([this] {
        this->_data->states.remove_state();
    });

    this->gui.get<tgui::Button>("select_button")->onPress([this] {
        if (this->new_path.empty())
        {
            this->advertisement = "The path is empty";
        }
        else
        {
            this->_data->paths.push_back(this->new_path);
            this->_data->states.remove_state();
        }
    });

    // if (!bus_stop_texture.loadFromFile("assets/img/bus_stop_texture.png"))
    // {
    //     throw GameException("Couldn't find file: assets/img/bus_stop_texture.png");
    // }

    // if (!curve_texture.loadFromFile("assets/img/bus_stop_sprites.png"))
    // {
    //     throw GameException("Couldn't find file: assets/img/bus_stop_sprites.png");
    // }

    // if (!traffic_light_texture.loadFromFile("assets/img/traffic_light_texture.png"))
    // {
    //     throw GameException("Couldn't find file: assets/img/person.png");
    // }

    // sf::View view(sf::FloatRect({0.f, 0.f}, {1000.f, 600.f}));
    // this->_data->window->setView(view);

    init_visual_elements();
}

void RouteSelect::update_inputs()
{
    sf::RenderTarget &target{*this->_data->window};
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->gui.handleEvent(*event);

        if (event->is<sf::Event::Closed>())
        {
            this->_data->window->close();
            break;
        }

        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if ((keyPress->button == sf::Mouse::Button::Left) && !sprite_pressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*this->_data->window);

                int i = 0; 
                for (auto element : visual_elements)
                {
                    if (element.first.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        i = element.second;
                        break;
                    }
                }

                for (auto element : this->_data->city.get_visual_elements())
                {
                    if (element->get_info()->get_id() == i)
                    {
                        add_to_path(element->get_info());
                    }
                }

                sprite_pressed = true; 

                return;
            }
        }

        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (sprite_pressed)
            {
                sprite_pressed = false; 
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
    this->_data->window->clear(sf::Color::White);

    // write text
    sf::Font font("assets/fonts/joystix.ttf");
    sf::Text text(font);

    gui.draw();

    // throws error if can't load font
    if (!font.openFromFile("assets/fonts/joystix.ttf"))
    {
        // error...
        throw GameException("Couldn't find file: assets/fonts/joystix.ttf");
    }

    for (auto visual_element : visual_elements)
    {
        this->_data->window->draw(visual_element.first);
    }

    draw_lines();

    sf::Text advertisement_text(font);
    advertisement_text.setString(advertisement);
    advertisement_text.setCharacterSize(12);
    advertisement_text.setPosition(sf::Vector2f(50.f, 500.f));
    advertisement_text.setFillColor(sf::Color::Black);

    this->_data->window->draw(advertisement_text);

    // Displays rendered objects
    this->_data->window->display();
}

void RouteSelect::init_visual_elements()
{
    for (auto visual_element : this->_data->city.get_visual_elements())
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());
        auto traffic_light = std::dynamic_pointer_cast<TrafficLight>(visual_element->get_info()); 

        if (bus_stop)
        {
            sf::Sprite sprite(bus_stop_texture);
            sprite.setPosition(sf::Vector2f(bus_stop->get_x(), bus_stop->get_y()));

            visual_elements.push_back(std::make_pair<sf::Sprite, int>(std::move(sprite), bus_stop->get_id())); 
        }
        else if (traffic_light)
        {
            sf::Sprite sprite(traffic_light_texture);
            sprite.setPosition(sf::Vector2f(traffic_light->get_x(), traffic_light->get_y()));

            visual_elements.push_back(std::make_pair<sf::Sprite, int>(std::move(sprite), traffic_light->get_id())); 
        }
        else
        {
            sf::Sprite sprite(curve_texture);
            sprite.setPosition(sf::Vector2f(visual_element->get_info()->get_x(), visual_element->get_info()->get_y()));

            visual_elements.push_back(std::make_pair<sf::Sprite, int>(std::move(sprite), visual_element->get_info()->get_id())); 
        }
    }
}

float calc_distance1(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

void RouteSelect::draw_lines()
{
    for (int i = 1; i < new_path.size(); i++)
    {
        auto element = new_path.at(i);
        auto element2 = new_path.at(i-1); 

        sf::Vertex ver1;
        ver1.position = sf::Vector2f(element->get_x(), element->get_y());
        ver1.color = sf::Color::Black;
        sf::Vertex ver2; 
        ver2.position = sf::Vector2f(element2->get_x(), element2->get_y());
        ver2.color = sf::Color::Black;

        sf::Vertex line[] = {ver1, ver2};

        this->_data->window->draw(line, 2, sf::PrimitiveType::Lines);
    };
}

void RouteSelect::add_to_path(std::shared_ptr<VisualElement> visual_element)
{
    if(new_path.empty())
    {
        new_path.push_back(visual_element);
        return;
    }

    bool connected = false;

    for (auto element : this->_data->city.get_streets())
    {
        if (element->get_src_node()->get_info()->get_id() == visual_element->get_id() && element->get_tgt_node()->get_info()->get_id() == new_path.back()->get_id())
        {
            connected = true;
        }
        if (element->get_tgt_node()->get_info()->get_id() == visual_element->get_id() && element->get_src_node()->get_info()->get_id() == new_path.back()->get_id())
        {
            connected = true;
        }
    }

    if (connected)
    {
        new_path.push_back(visual_element);
        advertisement = "";
    }
    else
    {
        advertisement = "The elements are not connected";
    }
}

RouteSelect::~RouteSelect()
{
}
