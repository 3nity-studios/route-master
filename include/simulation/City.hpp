#include <string>
#include <Designar/graph.hpp>
#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include "simulation/Street.hpp"

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

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    BusStopNodeList get_bus_stops() const noexcept;
    StreetArcList get_streets() const noexcept;
    
    void add_bus_stop(const BusStop& bus_stop);
    void add_street(const Street& street_info, const int& src_id, const int& tgt_id); 

    void initialize_bus_stops();
    std::list<std::pair<int, int>> run_simulation(Bus &bus, Employee &driver, int time, StreetArcList path);
};