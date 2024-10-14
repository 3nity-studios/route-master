#include "simulation/City.hpp"

City::City() : id(0), name(""), current_time(0)
{
    //empty
}

City::City(int _id, std::string _name, Designar::Graph<std::shared_ptr<VisualElement>, Street> _city_map, int _current_time)
    : id(_id), name(_name), city_map(_city_map), current_time(_current_time)
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

VisualElementNodeList City::get_visual_elements() const noexcept
{
    return city_map.nodes();
}

StreetArcList City::get_streets() const noexcept
{
    return city_map.arcs();
}

void City::add_bus_stop(const BusStop& bus_stop)
{
    city_map.insert_node(std::make_shared<BusStop>(bus_stop));
}

void City::add_street(const Street& street_info, const int& src_id, const int& tgt_id)
{   

    Designar::GraphNode<std::shared_ptr<VisualElement>, Street, Designar::EmptyClass> *src_visual_element = nullptr, *tgt_visual_element = nullptr;
    for(auto visual_element : city_map.nodes())
    {

        if(visual_element->get_info()->get_id() == src_id)
        {
            src_visual_element = visual_element;
        }
        else if(visual_element->get_info()->get_id() == tgt_id)
        {
            tgt_visual_element = visual_element;
        }
    }

    if (src_visual_element && tgt_visual_element)
    {
        city_map.insert_arc(src_visual_element, tgt_visual_element, street_info);
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
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(stop->get_info());

        if (bus_stop)
        {
            bus_stop->generate_passengers(current_time);
        }
    }
}

void City::update()
{
    current_time++;

    for(auto &stop : city_map.nodes())
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(stop->get_info());

        if (bus_stop)
        {
            bus_stop->update(current_time);
        }

        auto traffic_light = std::dynamic_pointer_cast<TrafficLight>(stop->get_info());

        if (traffic_light)
        {
            traffic_light->update(current_time);
        }
    }

    for(auto &street : city_map.arcs())
    {
        street->get_info().update();
    }
}

std::list<std::pair<int, int>> City::run_simulation(Bus &bus, Employee &driver, int time, StreetArcList path)
{
    std::list<std::pair<int, int>> times;

    bus.reset();
    int spent_time = 0;
    for(auto track : path)
    {
        update();

        auto bus_stop = std::dynamic_pointer_cast<BusStop>(path.get_first()->get_src_node()->get_info());
        auto traffic_light = std::dynamic_pointer_cast<TrafficLight>(path.get_first()->get_src_node()->get_info());

        if (bus_stop != nullptr)
        {
            bus.leave_passengers(*bus_stop);
            bus.add_passengers(time, *bus_stop);
            times.push_back(std::make_pair<int, int>(0, bus.get_time_in_bus_stop()));

            driver.calc_fatigue(track->get_info().get_distance());
            bus.calc_wear(track->get_info().get_distance());

            // TODO: Abort simulation if bus breaks or if driver is too tired

            int travel_time = track->get_info().get_travel_time();

            spent_time += travel_time;
            times.push_back(std::make_pair<int, int>(1, std::move(travel_time)));
        }
        else if (traffic_light != nullptr)
        {
            times.push_back(std::make_pair<int, int>(2, traffic_light->get_time_to_change()));
        }
        else
        {
            times.push_back(std::make_pair<int, int>(3, 0));
        }
    }

    return times;
}