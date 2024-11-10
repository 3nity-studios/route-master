#pragma once

#include "player/Achievement.hpp"
#include <vector>

class AchievementManager
{
  private:
    std::vector<Achievement> achievements;

  public:
    std::vector<Achievement> get_achievements() noexcept;
    void add_achievement(const Achievement &achievement);
    void update(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info);
};