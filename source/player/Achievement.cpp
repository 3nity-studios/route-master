#include "player/Achievement.hpp"

Achievement::Achievement() : id(0), name(""), description(""), current_level(0)
{
    // empty
}

Achievement::Achievement(
    const int &_id, const std::string &_name, const std::string &_description,
    std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>>
        _criteria, std::vector<std::function<int(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>>
        _progress,
    std::vector<int> _rewards)
    : id(_id), name(_name), description(_description), criteria(_criteria), progress(_progress), rewards(_rewards), current_level(0)
{
    // empty
}

// Getters
int Achievement::get_id() const
{
    return id;
}
std::string Achievement::get_name() const
{
    return name;
}
std::string Achievement::get_description() const
{
    return description;
}
std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>> Achievement::get_criteria() const
{
    return criteria;
}

std::vector<int> Achievement::get_rewards() const
{
    return rewards;
}
int Achievement::get_current_level() const
{
    return current_level;
}

void Achievement::set_id(const int &_id)
{
    id = _id;
}

void Achievement::set_name(const std::string &_name)
{
    name = _name;
}

void Achievement::set_description(const std::string &_description)
{
    description = _description;
}

void Achievement::set_criteria(const std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>> &_criteria)
{
    criteria = _criteria;
}

void Achievement::set_rewards(const std::vector<int> &_rewards)
{
    rewards = _rewards;
}

void Achievement::set_current_level(const int &_current_level)
{
    current_level = _current_level;
}

void Achievement::check_and_unlock(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)
{
    while (current_level < criteria.size() && criteria[current_level](player, store, simulation_info))
    {
        rewards[current_level];
        current_level++;
    }
}

int Achievement::get_progress(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)
{
    return progress[current_level](player, store, simulation_info);
}