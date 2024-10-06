#include "simulation/City.hpp"

City::City() : id(0), name("")
{
    //empty
}

City::City(int _id, std::string _name, Designar::Graph<BusStop, Street> _city_map)
    : id(_id), name(_name), city_map(_city_map)
{
    //empty
}

int City::get_id() const noexcept
{
    return id;
}

std::string City::get_name() const noexcept
{
    return name;
}

BusStopNodeList City::get_bus_stops() const noexcept
{
    return city_map.nodes();
}

StreetArcList City::get_streets() const noexcept
{
    return city_map.arcs();
}

void City::add_bus_stop(const BusStop& bus_stop)
{
    city_map.insert_node(bus_stop);
}

void City::add_street(const Street& street_info, const int& src_id, const int& tgt_id)
{   

    Designar::GraphNode<BusStop, Street, Designar::EmptyClass> *src_stop = nullptr, *tgt_stop = nullptr;
    for(auto stop : city_map.nodes())
    {
        if(stop->get_info().get_id() == src_id)
        {
            src_stop = stop;
        }
        else if(stop->get_info().get_id() == tgt_id)
        {
            tgt_stop = stop;
        }
    }

    if (src_stop && tgt_stop)
    {
        city_map.insert_arc(src_stop, tgt_stop, street_info);
    }
    else
    {
        throw std::runtime_error("Bus Stops not found");
    }
}


void City::initialize_bus_stops()
{

    for(auto &stop : city_map.nodes())
    {
        stop->get_info().generate_passengers();
    }
}

void City::update()
{
    for(auto &street : city_map.arcs())
    {
        street->get_info().update();
    }
}

std::list<std::pair<int, int>> City::run_simulation(Bus &bus, Employee &driver, int time, StreetArcList path)
{
    std::list<std::pair<int, int>> times;

    // BusStop current_stop = path.get_first()->get_src_node()->get_info();
    bus.reset();
    int spent_time = 0;
    for(auto track : path)
    {
        update();

        bus.leave_passengers(path.get_first()->get_src_node()->get_info());
        bus.add_passengers(time, path.get_first()->get_src_node()->get_info());
        times.push_back(std::make_pair<int, int>(0, bus.get_time_in_bus_stop()));

        driver.calc_fatigue(track->get_info().get_distance());
        bus.calc_wear(track->get_info().get_distance());
        // bus.move(track->get_info());

        // TODO: Abort simulation if bus breaks or if driver is too tired

        // current_stop = track->get_tgt_node()->get_info();

        int travel_time = track->get_info().get_travel_time();

        spent_time += travel_time;
        times.push_back(std::make_pair<int, int>(1, std::move(travel_time)));
    }

    return times;
}