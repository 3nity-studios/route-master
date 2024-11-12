#pragma once

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include "simulation/Street.hpp"
#include <Designar/graph.hpp>
#include <nlohmann/json.hpp>

using StreetArcList = Designar::SLList<Designar::GraphArc<Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass>, std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *>;

struct SimulationInfo
{
    Bus *bus;
    Employee *employee;
    std::vector<std::shared_ptr<VisualElement>> elements_path;
    std::pair<int, int> time_state;
    int path_index;
    bool next_is_street;
    bool route_completed;
    bool isVisible; 
    sf::Clock projection_clock;
    sf::Sprite projection_bus;
    sf::Vector2f projection_bus_speed;
    sf::Texture projection_bus_texture;
    float previous_time; 

    std::vector<std::string> track_names;
    std::vector<int> track_times;
    std::vector<std::string> passenger_stop_names;
    std::vector<std::pair<int, int>> passengers_per_stop;

    SimulationInfo(Bus *_bus, Employee *_employee, StreetArcList _path); 
        
    void set_path(StreetArcList path); 
    nlohmann::json to_json(); 
    sf::Vector2f calc_vector_tip(); 
    float get_elapsed_time(); 
};