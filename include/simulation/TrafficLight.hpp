#pragma once

#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include "simulation/VisualElement.hpp"

using StreetConnectionIDs = std::pair<int, int>;

class TrafficLight : public VisualElement
{
    private:
    std::vector<std::pair<StreetConnectionIDs, bool>> connections; //The second item of the pair is a bool that contains true if the light is green and false if the light is red
    int time_to_change;
    float last_change;

    public: 

    TrafficLight();
    TrafficLight(int _id, std::vector<std::pair<StreetConnectionIDs, bool>> _connections, int _time_to_change, float _x, float _y);
    TrafficLight(nlohmann::json j);

    void set_connections(std::vector<std::pair<StreetConnectionIDs, bool>> _connections);
    void set_time_to_change(int _time_to_change); 

    std::vector<std::pair<StreetConnectionIDs, bool>> get_connections();
    int get_time_to_change();

    void update(float current_time);

    nlohmann::json to_json();
};