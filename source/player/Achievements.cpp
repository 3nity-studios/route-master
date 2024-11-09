#include "player/Achievements.hpp"

Achievement::Achievement()
    : id(""), name(""), description(""), currentLevel(0)
{
    // empty
}

Achievement::Achievement(const std::string &id, const std::string &name, const std::string &description,
                         std::vector<std::function<bool(const GameData &)>> criteria,
                         std::vector<std::function<void()>> rewards)
    : id(id), name(name), description(description), criteria(criteria), rewards(rewards), currentLevel(0)
{
    // empty
}

void Achievement::check_and_unlock(const GameData &gameData)
{
    while (currentLevel < criteria.size() && criteria[currentLevel](gameData))
    {
        rewards[currentLevel]();
        currentLevel++;
    }
}