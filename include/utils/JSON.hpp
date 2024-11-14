#pragma once
#include "config/game.hpp"
#include "simulation/City.hpp"
#include "simulation/Street.hpp"
#include "simulation/VisualElement.hpp"
#include "simulation/Route.hpp"

namespace util
{
static SimulationInfo simulation_info_from_json(nlohmann::json j, GameDataRef _data)
{
    Bus aux_bus = j["bus"];
    Bus *j_bus = &_data->player.get_bus(aux_bus.get_id());
    Employee aux_employee = j["employee"];
    Employee *j_employee = &_data->player.get_employee(aux_employee.get_id()); 

    std::vector<VisualElement> elements_path_vector;

    for (auto visual_element : j["elements_path"])
    {
        elements_path_vector.push_back(visual_element); 
    }

    std::vector<std::shared_ptr<VisualElement>> j_elements_path; 

    for (auto visual_element : elements_path_vector)
    {
        for (auto element : _data->city.get_visual_elements())
        {
            if (visual_element.get_id() == element->get_info()->get_id())
            {
                j_elements_path.push_back(element->get_info());
                break;
            }
        }
    }

    SimulationInfo simulation_info(j_bus, j_employee, {});
    
    simulation_info.elements_path = j_elements_path;
    simulation_info.time_state.first = j["time_state_first"];
    simulation_info.time_state.second = j["time_state_second"];
    simulation_info.path_index = j["path_index"];
    simulation_info.next_is_street = j["next_is_street"];
    simulation_info.route_completed =  j["route_completed"]; 
    simulation_info.previous_time = j["previous_time"];
    simulation_info.isVisible = j["isVisible"];
    simulation_info.projection_bus.setPosition(sf::Vector2f(j["position_x"], j["position_y"]));
    simulation_info.projection_bus_speed = sf::Vector2f(j["speed_x"], j["speed_y"]);

    for (auto time : j["times"])
    {
        simulation_info.track_times.push_back(time);
    }

    for (auto passengers : j["passengers_stop_first"])
    {
        simulation_info.passengers_per_stop.push_back(std::make_pair<int, int>(passengers, 0));
    }

    int i = 0; 
    for (auto &passengers : simulation_info.passengers_per_stop)
    {
        passengers.second = j["passengers_stop_second"].at(i);
        i++;
    }

    simulation_info.projection_clock.restart();

    return simulation_info;
}

static nlohmann::json simulation_info_to_json(GameDataRef _data)
{
    nlohmann::json j; 

    nlohmann::json simulation_info = nlohmann::json::array(); 

    for (auto info : _data->simulation_info)
    {
        simulation_info.push_back(info.to_json());
    }

    j["simulation_info"] = simulation_info; 

    return j; 
}

static void save_simulation_info(GameDataRef _data)
{
    std::ofstream info_file("data/simulation_info.json");
    info_file << simulation_info_to_json(_data).dump(4);
    info_file.close();
}

static nlohmann::json route_vector_to_json(std::vector<Route> routes)
{
    nlohmann::json j; 

    nlohmann::json route_vector = nlohmann::json::array(); 

    for (auto route : routes)
    {
        route_vector.push_back(route.to_json());
    }

    j["route_vector"] = route_vector; 

    return j; 
}

static void save_route_vector(std::vector<Route> routes)
{
    std::ofstream route_file("data/routes.json");
    route_file << route_vector_to_json(routes).dump(4);
    route_file.close();
}

static std::vector<Route> route_vector_from_json(nlohmann::json j, std::vector<std::shared_ptr<VisualElement>> visual_elements)
{
    std::vector<Route> routes; 

    for (auto jroute : j["route_vector"])
    {
        routes.push_back(Route(jroute, visual_elements));
    }

    return routes;
}
} // namespace util