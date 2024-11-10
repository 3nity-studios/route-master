#include "states/achievements_state.hpp"
#include "config/game.hpp"
#include "config/global.hpp"
#include <string>

AchievementsState::AchievementsState(GameDataRef data) : _data(data), current_info(0)
{
    // empty
}

void AchievementsState::init_state()
{
    tgui::Theme::setDefault("assets/tgui/Kenney.txt");
    this->_data->gui.setWindow(*this->_data->window);
    this->_data->gui.loadWidgetsFromFile("assets/screens/basic_screen.txt");

    this->_data->gui.get<tgui::Label>("title")->setText("Achievements");

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] {
        this->_data->states.remove_state();
    });

    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    panel->setPosition({10, 90});

    auto achievements_list = tgui::ListBox::create();
    achievements_list->setSize({250.0f, 390.0f});
    achievements_list->setPosition({5.0f, 5.0f});
    panel->add(achievements_list);

    auto verticalLayout = tgui::VerticalLayout::create();
    verticalLayout->setSize({this->_data->window->getSize().x - 300.0f, 390.0f});
    verticalLayout->setPosition({260.0f, 5.0f});

    auto achievement_name = tgui::Label::create();
    achievement_name->setText("Achievement Name");
    achievement_name->getRenderer()->setTextColor(sf::Color::Black);
    achievement_name->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    verticalLayout->add(achievement_name);

    auto achievement_description = tgui::Label::create();
    achievement_description->setText("Description");
    achievement_description->getRenderer()->setTextColor(sf::Color::Black);
    achievement_description->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    verticalLayout->add(achievement_description);

    auto achievement_progress = tgui::ProgressBar::create();
    achievement_progress->setSize({this->_data->window->getSize().x - 200.0f, 20.0f});
    achievement_progress->setMinimum(0);
    achievement_progress->setMaximum(100);
    achievement_progress->setValue(50);
    achievement_progress->setText("50%");
    verticalLayout->add(achievement_progress);

    auto levels = tgui::HorizontalLayout::create();
    for(int i = 0; i < 3; ++i)
    {
        levels->add(tgui::Picture::create("assets/img/logo.png"));
    }
    verticalLayout->add(levels);

    panel->add(verticalLayout);
    this->_data->gui.add(panel);
}

void AchievementsState::update_inputs()
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
void AchievementsState::update_state(float dt __attribute__((unused)))
{
}

// marks dt to not warn compiler
void AchievementsState::draw_state(float dt __attribute__((unused)))
{
    this->_data->gui.draw();

    // Displays rendered objects
    this->_data->window->display();
}
