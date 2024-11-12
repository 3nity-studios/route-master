#include "simulation/TrafficLight.hpp"

TrafficLight::TrafficLight() : VisualElement(), connections(std::vector<std::pair<StreetConnectionIDs, bool>>{}), time_to_change(5) 
{
    //empty
}

TrafficLight::TrafficLight(int _id, std::vector<std::pair<StreetConnectionIDs, bool>> _connections, int _time_to_change, float _x, float _y) : VisualElement(_id, _x, _y), connections(_connections), time_to_change(_time_to_change)
{
    //empty
}

TrafficLight::TrafficLight(nlohmann::json j) : VisualElement(j)
{
    connections = j["connections"];
    time_to_change = j["time_to_change"];
}

void TrafficLight::set_connections(std::vector<std::pair<StreetConnectionIDs, bool>> _connections)
{
    connections = _connections;
}

void TrafficLight::set_time_to_change(int _time_to_change)
{
    time_to_change = _time_to_change;
}

std::vector<std::pair<StreetConnectionIDs, bool>> TrafficLight::get_connections()
{
    return connections;
}

int TrafficLight::get_time_to_change()
{
    return time_to_change;
}

void TrafficLight::update(float current_time)
{
    if (last_change + time_to_change/60.f < current_time)
    {
        return;
    }

    if (connections.back().second)
    {
        connections.back().second = false;
        connections.front().second = true;
    }
    else
    {
        int i = 0; 
        for (auto &connection : connections)
        {
            if (connection.second)
            {
                connection.second = false;
                i++;
                break; 
            }

            i++;
        }

        connections.at(i).second = true;
    }

    last_change = current_time;
}

nlohmann::json TrafficLight::to_json()
{
    nlohmann::json j = VisualElement::to_json();
    j["connections"] = connections;
    j["time_to_change"] = time_to_change;

    return j;
}
