#include <string>
#include <Designar/graph.hpp>
#include "Bus.hpp"
#include "Employee.hpp"
#include "Street.hpp"

using BusStopNodeList = Designar::SLList<Designar::GraphNode<BusStop, Street, Designar::EmptyClass> *>;
using StreetArcList = Designar::SLList<Designar::GraphArc<Designar::GraphNode<BusStop, Street, Designar::EmptyClass>, BusStop, Street, Designar::EmptyClass> *>;

class City
{
    private:
    int id;
    std::string name;
    Designar::Graph<BusStop, Street> city_map;

    public:
    City();
    City(int _id, std::string _name, Designar::Graph<BusStop, Street> _city_map);

    BusStopNodeList get_bus_stops() const noexcept;
    StreetArcList get_streets() const noexcept;
    
    void add_bus_stop(const BusStop& bus_stop);
    void add_street(const Street& street_info, const int& src_id, const int& tgt_id); 

    void initialize_bus_stops();
    void run_simulation(Bus &bus, Employee &driver, int time, StreetArcList path);
};