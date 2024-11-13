#include "states/messages_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

MessagesState::MessagesState(GameDataRef data) : _data(data)
{
    // empty
}

void MessagesState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Messages");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] { this->_data->states.remove_state(); });

    // Create a label for the player name
    auto playerInfoLabel = tgui::Label::create();
    playerInfoLabel->setText("Player: " + this->_data->player.get_name() + "\nBalance: $" +
                             std::to_string(this->_data->player.get_balance()));
    playerInfoLabel->getRenderer()->setTextColor(sf::Color::White);
    playerInfoLabel->getRenderer()->setTextOutlineColor(sf::Color::Black);
    playerInfoLabel->getRenderer()->setTextOutlineThickness(2);
    playerInfoLabel->setTextSize(16);
    playerInfoLabel->setPosition({this->_data->window->getSize().x - 200.0f, 20.0f});
    this->_data->gui.add(playerInfoLabel);

    // Create a panel to hold the messages
    messages_panel = tgui::ScrollablePanel::create();
    messages_panel->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
    messages_panel->setSize({this->_data->window->getSize().x - 20, 400});
    messages_panel->setPosition({10, 90});
    this->_data->gui.add(messages_panel);
}

void MessagesState::update_inputs()
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
void MessagesState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void MessagesState::draw_state(float dt __attribute__((unused)))
{
    this->_data->gui.draw();

    messages_panel->removeAllWidgets();
    for (int i = 0; i < this->_data->messages.size(); ++i)
    {
        messages_panel->add(create_message_box(this->_data->messages[i], i));
    }

    // Displays rendered objects
    this->_data->window->display();
}

tgui::Panel::Ptr MessagesState::create_message_box(Message &message, int message_number)
{
    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(50, 185, 242));
    panel->setSize({this->_data->window->getSize().x - 40, 120});
    panel->setPosition({10, 10 + (message_number * 130)});

    auto titleLabel = tgui::Label::create();
    titleLabel->setText(message.get_title());
    titleLabel->getRenderer()->setTextColor(sf::Color::Black);
    titleLabel->setTextSize(14);
    titleLabel->setPosition({10, 10});
    panel->add(titleLabel);

    auto contentLabel = tgui::Label::create();
    contentLabel->setText(message.get_content());
    contentLabel->getRenderer()->setTextColor(sf::Color::Black);
    contentLabel->setTextSize(14);
    contentLabel->setPosition({10, 40});
    panel->add(contentLabel);

    auto executeButton = tgui::Button::create();
    if(message.get_type() == MessageType::AchievementMessage)
    {
            executeButton->setText("Claim");
    }
    else
    {
            executeButton->setText("Pay");
    }	
    executeButton->setSize({150, 35});
    executeButton->setPosition({panel->getSize().x * 3 / 4, (panel->getSize().y - 35) / 2});
    executeButton->onPress([this, &message] {
        message.execute(this->_data->store, this->_data->player, this->_data->messages);
    });
    panel->add(executeButton);

    return panel;
}