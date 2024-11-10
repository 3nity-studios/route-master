#pragma once

#include "player/Achievement.hpp"
#include <vector>
#include <nlohmann/json.hpp>

class AchievementManager
{
  private:
    std::vector<Achievement> achievements;
    void to_json();

  public:
    std::vector<Achievement> get_achievements() noexcept;
    void add_achievement(const Achievement &achievement);
    void update(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info);
    void update_from_json();
};