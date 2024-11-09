#include "config/game.hpp"
#include <functional>
#include <string>
#include <vector>

class Achievement
{
  private:
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::function<bool(const GameData &)>> criteria;
    std::vector<std::function<void()>> rewards;
    int currentLevel;

  public:
    Achievement();
    Achievement(const std::string &id, const std::string &name, const std::string &description,
                std::vector<std::function<bool(const GameData &)>> criteria,
                std::vector<std::function<void()>> rewards);

    void check_and_unlock(const GameData &gameData);
};