#include "states/route_edit_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include "engine/input_manager.hpp"
#include "states/route_list_state.hpp"
#include "utils/calc_view.hpp"
#include <SFML/Window/Mouse.hpp>
#include <TGUI/Texture.hpp>
#include <cmath>
#include <string>
#include "utils/JSON.hpp"

RouteEditState::RouteEditState(GameDataRef data, Route &_route)
    : _data(data), map_icons_texture(sf::Image(sf::Vector2u(25, 25), sf::Color::Green)), sprite_pressed(false),
      route(_route), create_new_path(false)
{
    if (route.route.empty())
    {
        create_new_path = true; 
    }
    else
    {
        route_copy = route;
    }
}

void RouteEditState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");    
    this->_data->gui.setWindow(*this->_data->window);

    this->canvas = tgui::CanvasSFML::create({800, 600});
    this->_data->gui.add(this->canvas);
    
    auto nameLabel = tgui::Label::create();
    nameLabel->setText("Route Name:");
    nameLabel->setPosition({5.0f, 5.0f});
    nameLabel->setSize({150.0f, 30.0f});
    nameLabel->getRenderer()->setTextColor(sf::Color::White);
    this->_data->gui.add(nameLabel);

    auto nameEditBox = tgui::EditBox::create();
    nameEditBox->setSize({200.0f, 30.0f});
    nameEditBox->setPosition({150.0f, 5.0f});
    nameEditBox->setText(this->route.name);
    nameEditBox->onTextChange([this](const tgui::String &text) { this->route.name = text.toStdString(); });
    this->_data->gui.add(nameEditBox);

    auto select_button = tgui::Button::create();
    select_button->setText("Select");
    select_button->setWidgetName("select_button");
    select_button->setSize(180.f, 63.f);
    select_button->setPosition(100.f, 500.f);
    select_button->setRenderer(tgui::Theme::getDefault()->getRenderer("GreenButton"));
    this->_data->gui.add(select_button);

    auto cancel_button = tgui::Button::create();
    cancel_button->setWidgetName("cancel_button");
    cancel_button->setText("Cancel");
    cancel_button->setSize(180.f, 63.f);
    cancel_button->setPosition(300.f, 500.f);
    cancel_button->setRenderer(tgui::Theme::getDefault()->getRenderer("RedButton"));
    this->_data->gui.add(cancel_button);

    auto undoButton = tgui::Button::create();
    undoButton->setText("Undo");
    undoButton->setSize({100.0f, 30.0f});
    undoButton->setPosition({360.0f, 5.0f});
    undoButton->onPress([this] {
        if (!route.route.empty())
        {
            auto back = route.route.back();
            route.route.pop_back();
            redo_stack.push_back(back);
            for (auto &visual_element : visual_elements)
            {
                if (back->get_id() == std::get<1>(visual_element)->get_id())
                {
                    std::get<2>(visual_element) = false;
                    break;
                }
            }
        }
    });
    this->_data->gui.add(undoButton);

    auto redoButton = tgui::Button::create();
    redoButton->setText("Redo");
    redoButton->setSize({100.0f, 30.0f});
    redoButton->setPosition({470.0f, 5.0f});
    redoButton->onPress([this] {
        if (!redo_stack.empty())
        {
            auto back = redo_stack.back();
            redo_stack.pop_back();
            add_to_path(back);
            route.route.push_back(back);
            for (auto &visual_element : visual_elements)
            {
                if (back->get_id() == std::get<1>(visual_element)->get_id())
                {
                    std::get<2>(visual_element) = true;
                    break;
                }
            }
        }
    });
    this->_data->gui.add(redoButton);

    _view_dragger.emplace(this->canvas->getRenderTexture());
    this->_data->gui.get<tgui::Button>("cancel_button")->onPress([this] {
        if (this->create_new_path)
        {
            this->_data->routes.back().route.clear(); 
            this->_data->routes.pop_back(); 
        }
        else
        {
            this->route = route_copy;
        }
        util::save_route_vector(this->_data->routes);
        this->_data->states.add_state(Engine::StateRef(new RouteListState(this->_data)), true);
    });

    this->_data->gui.get<tgui::Button>("select_button")->onPress([this] {
        if (this->route.name == "" || this->route.route.empty())
        {
            auto messageBox = tgui::MessageBox::create();
            messageBox->setTitle("Warning");
            if (this->route.name == "")
            {
                messageBox->setText("Name cannot be empty");
            }
            else
            {
                messageBox->setText("The path is empty");
            }
            messageBox->addButton("OK");
            messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f,
                                    this->_data->window->getSize().y / 2 - 50.0f);
            messageBox->setSize(400.0f, 100.f);
            messageBox->onButtonPress([msgBox = messageBox.get()](const tgui::String &button) {
                msgBox->getParent()->remove(msgBox->shared_from_this());
            });
            this->_data->gui.add(messageBox);
        }
        else if (this->route.route.back()->get_id() != 36 && this->route.route.back()->get_id() != 37)
        {
            auto messageBox = tgui::MessageBox::create();
            messageBox->setTitle("Warning");
            messageBox->setText("The route must end at a bus station");
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
            util::save_route_vector(this->_data->routes);
            this->_data->states.add_state(Engine::StateRef(new RouteListState(this->_data)), true);
        }
    });

    if (!map_icons_texture.loadFromFile("assets/img/map_icons.png"))
    {
        throw GameException("Couldn't find file: assets/img/map_icons.png");
    }

    sf::View view;
    util::calc_view(this->canvas->getRenderTexture(), view);

    init_visual_elements();

    // this->_map.load("assets/maps/demo_editor.tmx");
    this->_data->assets.load_texture("editorBackground", "assets/maps/demo_editor.png");
    if (music.openFromFile("assets/music/Mellow-Mind_Looping.mp3"))
    {if (this->_data->is_music_on)
    {
        music.play();
        music.setLooping(true);
    }}
}

void RouteEditState::update_inputs()
{
    // Event Polling
    while (const std::optional event = this->_data->window->pollEvent())
    {
        this->_data->gui.handleEvent(*event);

        if (event->is<sf::Event::Closed>())
        {
            util::save_route_vector(this->_data->routes);
            this->_data->city.save();
            this->_data->window->close();
            break;
        }

        if (const auto *keyPress = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if ((keyPress->button == sf::Mouse::Button::Left) && !sprite_pressed)
            {
                for (auto &element : visual_elements)
                {
                    if (this->_data->inputs.is_sprite_clicked(std::get<0>(element), sf::Mouse::Button::Left,
                                                              *this->_data->window, this->canvas->getRenderTexture()))
                    {
                        // std::cout << "Detected a click on sprite at " <<
                        // sf::Mouse::getPosition(*this->_data->window).x  << ", " <<
                        // sf::Mouse::getPosition(*this->_data->window).y << std::endl;
                        if (add_to_path(std::get<1>(element)))
                        {
                            std::get<0>(element).setColor(sf::Color(255, 255, 255));
                            std::get<2>(element) = true;
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
            if (keyPress->code == sf::Keyboard::Key::Escape)
            {
                this->_data->states.remove_state();
                break;
            }
            // When Ctrl+Shift+Z is pressed, redo the last undone route addition
            if (keyPress->code == sf::Keyboard::Key::Z && keyPress->control && keyPress->shift)
            {
                if (!redo_stack.empty())
                {
                    auto back = redo_stack.back();
                    redo_stack.pop_back();
                    add_to_path(back);
                    route.route.push_back(back);
                    for (auto &visual_element : visual_elements)
                    {
                        if (back->get_id() == std::get<1>(visual_element)->get_id())
                        {
                            std::get<2>(visual_element) = true;
                            break;
                        }
                    }
                }
            }
            // When Ctrl+Z is pressed, undo the last route addition
            else if (keyPress->code == sf::Keyboard::Key::Z && keyPress->control)
            {
                if (!route.route.empty())
                {
                    auto back = route.route.back();
                    route.route.pop_back();
                    redo_stack.push_back(back);
                    for (auto &visual_element : visual_elements)
                    {
                        if (back->get_id() == std::get<1>(visual_element)->get_id())
                        {
                            std::get<2>(visual_element) = false;
                            break;
                        }
                    }
                }
            }
        }
        this->_view_dragger->handleEvent(*event);
    }
}

// marks dt to not warn compiler
void RouteEditState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void RouteEditState::draw_state(float dt __attribute__((unused)))
{
    // background color
    this->_data->gui.draw();

    // static MapLayer layerZero(_map, 0);
    // static MapLayer layerOne(_map, 1);
    // static MapLayer layerTwo(_map, 2);
    // static MapLayer layerThree(_map, 3);
    // static MapLayer layerFour(_map, 4);

    auto editorBackgroundTex = this->_data->assets.get_texture("editorBackground");
    auto editorBackground = sf::Sprite(editorBackgroundTex);
    this->canvas->clear(sf::Color::White);
    this->canvas->draw(editorBackground);

    // layerZero.draw(this->canvas->getRenderTexture(), sf::RenderStates::Default);
    // layerOne.draw(this->canvas->getRenderTexture(), sf::RenderStates::Default);
    // layerTwo.draw(this->canvas->getRenderTexture(), sf::RenderStates::Default);
    // layerThree.draw(this->canvas->getRenderTexture(), sf::RenderStates::Default);
    // layerFour.draw(this->canvas->getRenderTexture(), sf::RenderStates::Default);


    draw_lines();

    for (auto visual_element : visual_elements)
    {
        this->canvas->draw(std::get<0>(visual_element));
        // std::cout << "Created sprite at " << visual_element.first.getPosition().x << ", " <<
        // visual_element.first.getPosition().y << std::endl;
    }

    // Displays rendered objects
    this->_data->window->display();
    this->canvas->display();
}

void RouteEditState::init_visual_elements()
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
            sprite.setScale({0.7, 0.7});
            sprite.setPosition(sf::Vector2f(0.2*bus_stop->get_x(), 0.2*bus_stop->get_y()));
            sprite.setTextureRect(bus_stop_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_tuple<sf::Sprite, std::shared_ptr<VisualElement>, bool>(
                std::move(sprite), std::move(visual_element_pointer), false));
        }
        else if (traffic_light)
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setScale({0.7, 0.7});
            sprite.setPosition(sf::Vector2f(0.2*traffic_light->get_x(), 0.2*traffic_light->get_y()));
            sprite.setTextureRect(traffic_light_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_tuple<sf::Sprite, std::shared_ptr<VisualElement>, bool>(
                std::move(sprite), std::move(visual_element_pointer), false));
        }
        else
        {
            sf::Sprite sprite(map_icons_texture);
            sprite.setScale({0.7, 0.7});
            sprite.setPosition(sf::Vector2f(0.2*visual_element->get_info()->get_x(), 0.2*visual_element->get_info()->get_y()));
            sprite.setTextureRect(curve_rect);
            sprite.setOrigin(sprite.getLocalBounds().getCenter());
            sprite.setColor(sf::Color(75, 73, 71));

            visual_elements.push_back(std::make_tuple<sf::Sprite, std::shared_ptr<VisualElement>, bool>(
                std::move(sprite), std::move(visual_element_pointer), false));
        }

        if (!route.route.empty())
        {
            for (auto &visual_element2 : route.route)
            {
                if (visual_element2->get_id() == std::get<1>(visual_elements.back())->get_id())
                {
                    std::get<2>(visual_elements.back()) = true;
                }
            }
        }
    }
}

void RouteEditState::draw_lines()
{
    for (auto element : this->_data->city.get_streets())
    {
        sf::Vertex ver1;
        ver1.position =
            sf::Vector2f(0.2*element->get_src_node()->get_info()->get_x(), 0.2*element->get_src_node()->get_info()->get_y());
        ver1.color = sf::Color::Black;
        sf::Vertex ver2;
        ver2.position =
            sf::Vector2f(0.2*element->get_tgt_node()->get_info()->get_x(), 0.2*element->get_tgt_node()->get_info()->get_y());
        ver2.color = sf::Color::Black;

        sf::Vertex line[] = {ver1, ver2};

        this->canvas->draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (int i = 1; i < route.route.size(); i++)
    {
        auto element = route.route.at(i);
        auto element2 = route.route.at(i - 1);

        sf::Vertex ver1;
        ver1.position = sf::Vector2f(0.2*element->get_x(), 0.2*element->get_y());
        ver1.color = sf::Color::Yellow;
        sf::Vertex ver2;
        ver2.position = sf::Vector2f(0.2*element2->get_x(), 0.2*element2->get_y());
        ver2.color = sf::Color::Yellow;

        sf::Vertex line[] = {ver1, ver2};

        this->canvas->draw(line, 2, sf::PrimitiveType::Lines);
    };

    for (auto &visual_element : visual_elements)
    {
        if (std::get<2>(visual_element))
        {
            std::get<0>(visual_element).setColor(sf::Color(255, 255, 255));
        }
        else
        {
            std::get<0>(visual_element).setColor(sf::Color(75, 73, 71));
        }
        //this the actual position clicks are getting checked against
        //fix asap
        // auto collisionRect = sf::RectangleShape(std::get<0>(visual_element).getGlobalBounds().size);
        // collisionRect.setFillColor(sf::Color(100, 250, 50));
        // collisionRect.move(std::get<0>(visual_element).getGlobalBounds().position);
        // this->_data->window->draw(collisionRect);
    }
}

bool RouteEditState::add_to_path(std::shared_ptr<VisualElement> visual_element)
{
    if (route.route.empty() && (visual_element->get_id() == 36 || visual_element->get_id() == 37))
    {
        route.route.push_back(visual_element);
        return true;
    }
    else if (route.route.empty() && (visual_element->get_id() != 36 || visual_element->get_id() != 37))
    {
        auto messageBox = tgui::MessageBox::create();
        messageBox->setTitle("Warning");
        messageBox->setText("The route must start at a bus station");
        messageBox->addButton("OK");
        messageBox->setPosition(this->_data->window->getSize().x / 2 - 200.0f,
                                this->_data->window->getSize().y / 2 - 50.0f);
        messageBox->setSize(400.0f, 100.f);
        messageBox->onButtonPress([msgBox = messageBox.get()](const tgui::String &button) {
            msgBox->getParent()->remove(msgBox->shared_from_this());
        });
        this->_data->gui.add(messageBox);

        return false; 
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

RouteEditState::~RouteEditState()
{
}
