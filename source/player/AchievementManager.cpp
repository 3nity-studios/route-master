#include "player/AchievementManager.hpp"

std::vector<Achievement> AchievementManager::get_achievements() noexcept
{
    return achievements;
}

void AchievementManager::add_achievement(const Achievement &achievement)
{
    achievements.push_back(achievement);
}

void AchievementManager::update(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)
{
    for (Achievement &achievement : achievements)
    {
        achievement.check_and_unlock(player, store, simulation_info);
    }
}