#include <City.hpp>

City::City() : id(0), name("")
{
    //empty
}

City::City(int _id, std::string _name, Designar::Graph<BusStop, Street> _city_map)
    : id(_id), name(_name), city_map(_city_map)
{
    //empty
}

Designar::SLList<Designar::GraphNode<BusStop, Street, Designar::EmptyClass> *> City::get_bus_stops()
{
    return city_map.nodes();
}

Designar::SLList<Designar::GraphArc<Designar::GraphNode<BusStop, Street, Designar::EmptyClass>, BusStop, Street, Designar::EmptyClass> *> City::get_streets()
{
    return city_map.arcs();
}

void City::add_bus_stop(BusStop bus_stop)
{
    city_map.insert_node(bus_stop);
}

void City::add_street(Street street_info, int src_id, int tgt_id)
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
    auto streets = city_map.arcs();

    for(auto stop : city_map.nodes())
    {
        stop->get_info().generate_passengers();
    }
}

void City::run_simulation(Bus &bus, Employee &driver, int time, std::list<Designar::GraphArc<Designar::GraphNode<BusStop, Street, Designar::EmptyClass>, BusStop, Street, Designar::EmptyClass> *> path)
{
    auto current_stop = path.front()->get_src_node()->get_info();
    int spent_time = 0;
    for(auto track : path)
    {
        bus.leave_passengers(current_stop.get_id());
        bus.add_passengers(time, current_stop);

        driver.calc_fatigue(track->get_info().get_distance());
        // bus.move(track->get_info());

        // TODO: Abort simulation if bus breaks or if driver is too tired

        current_stop = track->get_tgt_node()->get_info();
        spent_time += track->get_info().get_travel_time();
    }
}