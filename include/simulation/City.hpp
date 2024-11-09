#pragma once

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include "simulation/Street.hpp"
#include "simulation/TrafficLight.hpp"
#include <Designar/graph.hpp>
#include <nlohmann/json.hpp>
#include <string>

using VisualElementNodeList = Designar::SLList<Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *>;
using StreetArcList = Designar::SLList<Designar::GraphArc<Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass>, std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *>;

struct SimulationInfo
{
    Bus *bus;
    Employee *employee;
    std::vector<std::shared_ptr<VisualElement>> elements_path;
    std::pair<int, int> time_state;
    int path_index;
    int previous_time;
    bool next_is_street;
    bool route_completed;
    bool isVisible; 
    sf::Clock projection_clock;
    sf::Sprite projection_bus;
    sf::Vector2f projection_bus_speed;
    sf::Texture projection_bus_texture;

    std::vector<std::string> track_names;
    std::vector<int> track_times;
    std::vector<std::string> passenger_stop_names;
    std::vector<std::pair<int, int>> passengers_per_stop;

    SimulationInfo(Bus *_bus, Employee *_employee, StreetArcList _path)
        : bus(_bus), employee(_employee), time_state(std::make_pair<int, int>(-1, 0)), path_index(0),
          next_is_street(false), route_completed(false), projection_bus_texture(sf::Texture()),
          projection_bus(projection_bus_texture)
    {
        set_path(_path);
    }
    void set_path(StreetArcList path)
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

    sf::Vector2f calc_vector_tip()
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
};

struct Route
{
    std::string name;
    std::vector<std::shared_ptr<VisualElement>> route;

    Route()
    {
        name = "";
    }
    
    Route(std::string _name, std::vector<std::shared_ptr<VisualElement>> _route)
    {
        name = _name;
        route = _route;
    }

    void add_visual_element(std::shared_ptr<VisualElement> element)
    {
        route.push_back(element);
    }
};

class City
{
  private:
    int id;
    std::string name;
    Designar::Graph<std::shared_ptr<VisualElement>, Street> city_map;
    int current_time;
    std::vector<int> current_passengers;

  public:
    City();
    City(int _id, std::string _name, Designar::Graph<std::shared_ptr<VisualElement>, Street> _city_map,
         int _current_time);
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

    void initialize_bus_stops();
    void update();
    void run_simulation(std::vector<SimulationInfo> &simulation_infos);

    nlohmann::json to_json();
};