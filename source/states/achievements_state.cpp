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

    this->_data->gui.get<tgui::Button>("exit_button")->onPress([this] { this->_data->states.remove_state(); });

    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBackgroundColor(tgui::Color(217, 217, 217));
    panel->setSize({this->_data->window->getSize().x - 20.0f, 400.0f});
    panel->setPosition({10, 90});

    auto verticalLayout = tgui::VerticalLayout::create();
    verticalLayout->setSize({this->_data->window->getSize().x - 300.0f, 390.0f});
    verticalLayout->setPosition({260.0f, 5.0f});

    auto achievement_name = tgui::Label::create();
    achievement_name->getRenderer()->setTextColor(sf::Color::White);
    verticalLayout->add(achievement_name);

    auto achievement_description = tgui::Label::create();
    achievement_description->getRenderer()->setTextColor(sf::Color::White);
    verticalLayout->add(achievement_description);

    auto achievement_progress = tgui::ProgressBar::create();
    achievement_progress->setSize({this->_data->window->getSize().x - 200.0f, 20.0f}); //this not respected
    achievement_progress->setMinimum(0);
    achievement_progress->setMaximum(100);
    achievement_progress->setValue(50);
    verticalLayout->add(achievement_progress);

    auto levels = tgui::HorizontalLayout::create();
    verticalLayout->add(levels);

    auto achievements_list = tgui::ListBox::create();
    achievements_list->setSize({250.0f, 390.0f});
    achievements_list->setPosition({5.0f, 5.0f});
    for (int i = 0; i < this->_data->achievement_manager.get_achievements().size(); ++i)
    {
        auto achievement = this->_data->achievement_manager.get_achievements()[i];
        achievements_list->addItem(achievement.get_name(), std::to_string(i));
    }
    achievements_list->onItemSelect([this, achievement_name, achievement_description, achievement_progress,
                                     levels](int index) {
        if (index >= 0)
        {
            auto achievement = this->_data->achievement_manager.get_achievements()[index];
            achievement_name->setText(achievement.get_name());
            achievement_description->setText(achievement.get_description());
            achievement_progress->setValue(
                achievement.get_progress(this->_data->player, this->_data->store, this->_data->simulation_info));
            achievement_progress->setText(std::to_string(achievement.get_progress(
                                              this->_data->player, this->_data->store, this->_data->simulation_info)) +
                                          "%");

            levels->removeAllWidgets();

            switch (achievement.get_current_level())
            {
            case 1:
                levels->add(tgui::Picture::create("assets/img/medals/bronze.png"));
                levels->add(tgui::Picture::create("assets/img/medals/off.png"));
                levels->add(tgui::Picture::create("assets/img/medals/off.png"));
                break;
            case 2:
                levels->add(tgui::Picture::create("assets/img/medals/bronze.png"));
                levels->add(tgui::Picture::create("assets/img/medals/silver.png"));
                levels->add(tgui::Picture::create("assets/img/medals/off.png"));
                break;
            case 3:
                levels->add(tgui::Picture::create("assets/img/medals/bronze.png"));
                levels->add(tgui::Picture::create("assets/img/medals/silver.png"));
                levels->add(tgui::Picture::create("assets/img/medals/gold.png"));
                break;
            default:
                levels->add(tgui::Picture::create("assets/img/medals/off.png"));
                levels->add(tgui::Picture::create("assets/img/medals/off.png"));
                levels->add(tgui::Picture::create("assets/img/medals/off.png"));
                break;
            }
        }
    });
    achievements_list->setSelectedItemById("0");
    panel->add(achievements_list);

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
