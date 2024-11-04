#include "states/route_select.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>
#include <cmath>
#include "states/bus_select_state.hpp"

RouteSelect::RouteSelect(GameDataRef data) : _data(data), map_icons_texture(sf::Image(sf::Vector2u(25, 25), sf::Color::Green)), sprite_pressed(false)
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

    if (!map_icons_texture.loadFromFile("assets/img/map_icons.png"))
    {
        throw GameException("Couldn't find file: assets/img/map_icons.png");
    }

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

                for (auto &element : visual_elements)
                {
                    if (element.first.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        if (add_to_path(element.second))
                        {
                            element.first.setColor(sf::Color(255,255,255));
                        }
                    
                        break;
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

    draw_lines();

    for (auto visual_element : visual_elements)
    {
        this->_data->window->draw(visual_element.first);
    }

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
    sf::IntRect traffic_light_rect(sf::Vector2i(0,0), sf::Vector2i(27,53));
    sf::IntRect bus_stop_rect(sf::Vector2i(44,10), sf::Vector2i(52,52));
    sf::IntRect curve_rect(sf::Vector2i(98,11), sf::Vector2i(53,52));

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

            visual_elements.push_back(std::make_pair<sf::Sprite, std::shared_ptr<VisualElement>>(std::move(sprite), std::move(visual_element_pointer))); 
        }
        else if (traffic_light)
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setPosition(sf::Vector2f(traffic_light->get_x(), traffic_light->get_y()));
            sprite.setTextureRect(traffic_light_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_pair<sf::Sprite, std::shared_ptr<VisualElement>>(std::move(sprite), std::move(visual_element_pointer))); 
        }
        else
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setPosition(sf::Vector2f(visual_element->get_info()->get_x(), visual_element->get_info()->get_y()));
            sprite.setTextureRect(curve_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_pair<sf::Sprite, std::shared_ptr<VisualElement>>(std::move(sprite), std::move(visual_element_pointer))); 
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
        ver1.position = sf::Vector2f(element->get_src_node()->get_info()->get_x(), element->get_src_node()->get_info()->get_y());
        ver1.color = sf::Color::Black;
        sf::Vertex ver2; 
        ver2.position = sf::Vector2f(element->get_tgt_node()->get_info()->get_x(), element->get_tgt_node()->get_info()->get_y());
        ver2.color = sf::Color::Black;

        sf::Vertex line[] = {ver1, ver2};

        this->_data->window->draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (int i = 1; i < new_path.size(); i++)
    {
        auto element = new_path.at(i);
        auto element2 = new_path.at(i-1); 

        sf::Vertex ver1;
        ver1.position = sf::Vector2f(element->get_x(), element->get_y());
        ver1.color = sf::Color::Yellow;
        sf::Vertex ver2; 
        ver2.position = sf::Vector2f(element2->get_x(), element2->get_y());
        ver2.color = sf::Color::Yellow;

        sf::Vertex line[] = {ver1, ver2};

        this->_data->window->draw(line, 2, sf::PrimitiveType::Lines);
    };
}

bool RouteSelect::add_to_path(std::shared_ptr<VisualElement> visual_element)
{
    if(new_path.empty())
    {
        new_path.push_back(visual_element);
        return true;
    }

    bool connected = false;

    for (auto element : this->_data->city.get_streets())
    {
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

    return connected;
}

RouteSelect::~RouteSelect()
{
}
