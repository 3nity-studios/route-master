#include "player/AchievementMessage.hpp"
#include "player/Achievement.hpp"


AchievementMessage::AchievementMessage(Achievement achievement)
{
    title = "New level achieved in " + achievement.get_name();
    content = "Level " + std::to_string(achievement.get_current_level() + 1) + " achieved in " + achievement.get_name();
    type = MessageType::AchievementMessage;
}

void AchievementMessage::execute(Achievement achievement, Player player)
{
    player.increase_balance(achievement.get_rewards()[achievement.get_current_level()]);
}