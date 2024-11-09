#include "player/AchievementManager.hpp"

void AchievementManager::add_achievement(const Achievement &achievement)
{
    achievements.push_back(achievement);
}

void AchievementManager::update(const GameData& gameData)
{
    for (Achievement &achievement : achievements)
    {
        achievement.check_and_unlock(gameData);
    }
}