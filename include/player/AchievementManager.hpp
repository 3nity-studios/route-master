#include "config/game.hpp"
#include "player/Achievements.hpp"
#include <functional>
#include <string>
#include <vector>

class AchievementManager
{
  private:
    std::vector<Achievement> achievements;

  public:
    void add_achievement(const Achievement &achievement);
    void update(const GameData& gameData);
};