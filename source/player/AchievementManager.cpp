#include "player/AchievementManager.hpp"

std::vector<Achievement> AchievementManager::get_achievements() noexcept
{
    return achievements;
}

void AchievementManager::add_achievement(const Achievement &achievement)
{
    achievements.push_back(achievement);
}

void AchievementManager::to_json()
{
    nlohmann::json json_achievements = nlohmann::json::array();

    for (const auto &achievement : achievements)
    {
        json_achievements.push_back({{"id", achievement.get_id()}, {"level", achievement.get_current_level()}});
    }

    std::ofstream file("data/achievements.json");
    if (file.is_open())
    {
        file << json_achievements.dump(4); // Pretty print with 4 spaces
        file.close();
    }
}
void AchievementManager::update_from_json()
{
    std::ifstream file("data/achievements.json");
    if (file.is_open())
    {
        nlohmann::json json_achievements;
        file >> json_achievements;
        file.close();
        
        int i = 0;
        for (const auto &json_achievement : json_achievements)
        {
            achievements[i].set_id(json_achievement.at("id").get<int>());
            achievements[i].set_current_level(json_achievement.at("level").get<int>());
            ++i;
        }
    }
}

void AchievementManager::update(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)
{
    for (Achievement &achievement : achievements)
    {
        achievement.check_and_unlock(player, store, simulation_info);
    }
    to_json();
}