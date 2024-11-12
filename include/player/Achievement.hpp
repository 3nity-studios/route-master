#pragma once

#include "player/Player.hpp"
#include "player/Store.hpp"
#include "simulation/City.hpp"
#include <functional>
#include <string>
#include <vector>

class Achievement
{
  private:
    int id;
    std::string name;
    std::string description;
    std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>>
        criteria;
    std::vector<std::function<int(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>>
        progress;
    std::vector<int> rewards;
    int current_level;
    bool completed;

  public:
    Achievement();
    Achievement(
        const int &_id, const std::string &_name, const std::string &_description,
        std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>>
            _criteria, std::vector<std::function<int(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>>
        _progress,
        std::vector<int> _rewards);

    int get_id() const;
    std::string get_name() const;
    std::string get_description() const;
    std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>> get_criteria() const;
    std::vector<int> get_rewards() const;
    int get_current_level() const;
    bool is_completed() const;

    void set_id(const int &_id);
    void set_name(const std::string &_name);
    void set_description(const std::string &_description);
    void set_criteria(const std::vector<std::function<bool(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info)>> &_criteria);
    void set_rewards(const std::vector<int> &_rewards);
    void set_current_level(const int &_current_level);

    void check_and_unlock(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info);
    int get_progress(Player &player, Store &store, std::vector<SimulationInfo> &simulation_info);
};