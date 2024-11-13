#include "simulation/SimulationInfo.hpp"

SimulationInfo::SimulationInfo(Bus *_bus, Employee *_employee, StreetArcList _path) : bus(_bus), employee(_employee), time_state(std::make_pair<int, int>(-1, 0)), path_index(0),
          next_is_street(false), route_completed(false), projection_bus_texture(sf::Texture()),
          projection_bus(projection_bus_texture), isVisible(true), previous_time(0)
{
    set_path(_path);
}

void SimulationInfo::set_path(StreetArcList path)
{
    elements_path.clear();

    if (!path.is_empty())
    {
        for (auto track : path)
        {
            elements_path.push_back(track->get_src_node()->get_info());
        }

        elements_path.push_back(path.get_last()->get_tgt_node()->get_info());
    }
}

nlohmann::json SimulationInfo::to_json()
{
    projection_clock.stop(); 
    previous_time = projection_clock.getElapsedTime().asSeconds();

    nlohmann::json j; 

    j["bus"] = bus->to_json();
    j["employee"] = employee->to_json(); 
       
    nlohmann::json elements_path_json = nlohmann::json::array();
    for (auto visual_element : elements_path)
    {
        elements_path_json.push_back(visual_element->to_json());
    }

    j["elements_path"] = elements_path_json;
    j["time_state_first"] = time_state.first;
    j["time_state_second"] = time_state.second;
    j["path_index"] = path_index; 
    j["next_is_street"] = next_is_street;
    j["route_completed"] = route_completed; 
    j["previous_time"] = previous_time;
    j["isVisible"] = isVisible; 

    nlohmann::json times_vector = nlohmann::json::array();
    for (auto time : track_times)
    {
        times_vector.push_back(time);
    }

    j["times"] = times_vector;

    nlohmann::json passengers_per_stop_first = nlohmann::json::array();
    nlohmann::json passengers_per_stop_second = nlohmann::json::array(); 

    for (auto passengers : passengers_per_stop)
    {
        passengers_per_stop_first.push_back(passengers.first);
        passengers_per_stop_second.push_back(passengers.second);
    }

    j["passengers_stop_first"] = passengers_per_stop_first;
    j["passengers_stop_second"] = passengers_per_stop_second;
    j["position_x"] = projection_bus.getPosition().x;
    j["position_y"] = projection_bus.getPosition().y;
    j["speed_x"] = projection_bus_speed.x;
    j["speed_y"] = projection_bus_speed.y; 

    return j; 
}

sf::Vector2f SimulationInfo::calc_vector_tip() 
{
    float bus_speed_module = sqrt((projection_bus_speed.x * projection_bus_speed.x) + (projection_bus_speed.y * projection_bus_speed.y));

    sf::Vector2f norm_direction_vector;

    if (bus_speed_module == 0)
    {
        norm_direction_vector = sf::Vector2f(0.f, 0.f);
    }
    else
    {
        norm_direction_vector = projection_bus_speed / bus_speed_module;
    }

    sf::Vector2f position_vector_tip = projection_bus.getPosition() + (norm_direction_vector * 60.f);

    return position_vector_tip;
}

float SimulationInfo::get_elapsed_time()
{
    return previous_time + projection_clock.getElapsedTime().asSeconds();
}