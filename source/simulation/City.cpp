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

void City::add_traffic_light(TrafficLight traffic_light)
{
    city_map.insert_node(std::make_shared<TrafficLight>(traffic_light));
}

void City::add_curve(const VisualElement &curve)
{
    city_map.insert_node(std::make_shared<VisualElement>(curve));
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

void City::run_simulation(std::vector<SimulationInfo> &simulation_infos)
{
    for (auto &simulation_info : simulation_infos)
    {
        if (simulation_info.route_completed)
        {
            continue;
        }

        auto bus_stop = std::dynamic_pointer_cast<BusStop>(simulation_info.elements_path.at(simulation_info.path_index));
        auto traffic_light = std::dynamic_pointer_cast<TrafficLight>(simulation_info.elements_path.at(simulation_info.path_index));

        if (simulation_info.time_state.first == -1)
        {
            simulation_info.time_state = std::make_pair<int, int>(0, simulation_info.bus.get_time_in_bus_stop());
            simulation_info.previous_time = current_time;
            simulation_info.next_is_street = true;
            simulation_info.bus.leave_passengers(*bus_stop);
            simulation_info.bus.add_passengers(current_time, *bus_stop);
            simulation_info.projection_clock.restart();
        }
        else if (simulation_info.projection_clock.getElapsedTime().asSeconds() <= simulation_info.time_state.second)
        {
            continue;
        }
        else if (simulation_info.next_is_street)
        {
            if (simulation_info.elements_path.size() - 1 == simulation_info.path_index)
            {
                simulation_info.route_completed = true; 
                simulation_info.time_state = std::make_pair<int, int>(0, 0);
                continue;
            }

            for (auto track : get_streets())
            {
                if (track->get_src_node()->get_info()->get_id() == simulation_info.elements_path.at(simulation_info.path_index)->get_id() && track->get_tgt_node()->get_info()->get_id() == simulation_info.elements_path.at(simulation_info.path_index + 1)->get_id())
                {
                    simulation_info.time_state = std::make_pair<int, int>(1, track->get_info().get_travel_time());
                    simulation_info.employee.calc_fatigue(track->get_info().get_distance());
                    simulation_info.bus.calc_wear(track->get_info().get_distance());
                    break;
                }
            }

            simulation_info.next_is_street = false;
            simulation_info.previous_time = current_time;
            simulation_info.path_index++;
            simulation_info.projection_clock.restart();
        }
        else if (bus_stop)
        {
            simulation_info.time_state = std::make_pair<int, int>(0, simulation_info.bus.get_time_in_bus_stop());
            simulation_info.previous_time = current_time;
            simulation_info.next_is_street = true;
            simulation_info.bus.leave_passengers(*bus_stop);
            simulation_info.bus.add_passengers(current_time, *bus_stop);
            simulation_info.projection_clock.restart();
        }
        else if (traffic_light)
        {
            simulation_info.time_state = std::make_pair<int, int>(2, traffic_light->get_time_to_change());
            simulation_info.next_is_street = true;
            simulation_info.previous_time = current_time;
            simulation_info.projection_clock.restart();
        }
        else
        {
            simulation_info.time_state = std::make_pair<int, int>(3, 0);
            simulation_info.next_is_street = true;
            simulation_info.previous_time = current_time;
            simulation_info.projection_clock.restart();
        }
    }

    update_passengers();
}

std::vector<int> City::get_current_passengers()
{
    return current_passengers;
}

void City::set_current_passengers(std::vector<int> _current_passengers)
{
    current_passengers = _current_passengers;
}

void City::update_passengers()
{
    current_passengers.clear();

    for (auto visual_element : get_visual_elements())
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (bus_stop)
        {
            current_passengers.push_back(bus_stop->get_passenger_list().size());
        }
    }
}