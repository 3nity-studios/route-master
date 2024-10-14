#pragma once

#include <string>
#include <Designar/graph.hpp>
#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include "simulation/Street.hpp"
#include "simulation/TrafficLight.hpp"

using VisualElementNodeList = Designar::SLList<Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *>;
using StreetArcList = Designar::SLList<Designar::GraphArc<Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass>, std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *>;

class City
{
    private:
    int id;
    std::string name;
    Designar::Graph<std::shared_ptr<VisualElement>, Street> city_map;
    int current_time;

    public:
    City();
    City(int _id, std::string _name, Designar::Graph<std::shared_ptr<VisualElement>, Street> _city_map, int _current_time);

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    VisualElementNodeList get_visual_elements() const noexcept;
    StreetArcList get_streets() const noexcept;
    
    void add_bus_stop(const BusStop& bus_stop);
    void add_street(const Street& street_info, const int& src_id, const int& tgt_id); 

    void initialize_bus_stops();
    void update();
    std::list<std::pair<int, int>> run_simulation(Bus &bus, Employee &driver, int time, StreetArcList path);
};