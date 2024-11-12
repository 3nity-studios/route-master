#pragma once

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include "simulation/Street.hpp"
#include "simulation/TrafficLight.hpp"
#include <string>
#include "simulation/Route.hpp"
#include "simulation/SimulationInfo.hpp"

using VisualElementNodeList = Designar::SLList<Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *>;

class City
{
  private:
    int id;
    std::string name;
    Designar::Graph<std::shared_ptr<VisualElement>, Street> city_map;
    std::vector<int> current_passengers;
    int current_time_hours; 
    int current_time_minutes; 
    int current_time_day; 

  public:
    City();
    City(int _id, std::string _name, Designar::Graph<std::shared_ptr<VisualElement>, Street> _city_map,
         int _current_time_day);
    City(nlohmann::json j);

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    VisualElementNodeList get_visual_elements() const noexcept;
    StreetArcList get_streets() const noexcept;
    std::vector<int> get_current_passengers();
    Designar::Graph<std::shared_ptr<VisualElement>, Street> get_city_map();

    void add_bus_stop(const BusStop &bus_stop);
    void add_street(const Street &street_info, const int &src_id, const int &tgt_id);
    void add_traffic_light(TrafficLight traffic_light);
    void add_curve(const VisualElement &visual_element);
    void set_current_passengers(std::vector<int> _current_passengers);
    void update_passengers();

    void initialize_bus_stops(int current_time_day);
    void update();
    void run_simulation(std::vector<SimulationInfo> &simulation_infos);

    nlohmann::json to_json();
    void save(); 
};