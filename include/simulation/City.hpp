#include <string>
#include <graph.hpp>
#include "BusStop.hpp"
#include "Bus.hpp"
#include "Employee.hpp"

class City
{
    private:
    int id;
    std::string name;
    Designar::Graph<BusStop, int> city_map;

    public:
    City();
    City(int _id, std::string _name, Designar::Graph<BusStop, int> _city_map);

    Designar::SLList<Designar::GraphNode<BusStop, int, Designar::EmptyClass> *> City::get_bus_stops();
    Designar::SLList<Designar::GraphArc<Designar::GraphNode<BusStop, int, Designar::EmptyClass>, BusStop, int, Designar::EmptyClass> *> City::get_streets();
    
    
    void add_bus_stop(BusStop bus_stop);
    void add_street(int distance, int src_id, int tgt_id); 

    void initialize_bus_stops();
    void run_simulation(Bus &bus, Employee &driver, int time, std::list<Designar::GraphArc<Designar::GraphNode<BusStop, int, Designar::EmptyClass>, BusStop, int, Designar::EmptyClass> *> path);
};