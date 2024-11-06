#include "simulation/Bus.hpp"

Bus::Bus() : id(0), name(""), max_capacity(0), current_passengers(std::list<Passenger>{}), time_in_bus_stop(0), engine_state(100), breaks_state(100), tires_state(100), fuel(100), in_route(false)
{
    //empty
}

Bus::Bus(int _id, std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop) 
    : id(_id), name(_name), max_capacity(_max_capacity), current_passengers(_current_passengers), time_in_bus_stop(_time_in_bus_stop), engine_state(100), breaks_state(100), tires_state(100), fuel(100), in_route(false)
{
    //empty
}

int Bus::get_id() const noexcept
{
    return id;
}

void Bus::set_id(const int& _id)
{
    id = _id;
}

std::string Bus::get_name() const noexcept
{
    return name; 
}

int Bus::get_max_capacity() const noexcept
{
    return max_capacity;
}

std::list<Passenger> Bus::get_passenger_list() const noexcept
{
    return current_passengers;
}

int Bus::get_time_in_bus_stop() const noexcept
{
    return time_in_bus_stop;
}

int Bus::get_engine_state() const noexcept
{
    return engine_state;
}

int Bus::get_breaks_state() const noexcept
{
    return breaks_state;
}

int Bus::get_tires_state() const noexcept
{
    return tires_state;
}

int Bus::get_fuel() const noexcept
{
    return fuel;
}

int Bus::leave_passengers(BusStop &current_stop)
{
    std::list<Passenger> aux;
    int passengers = 0; 

    for (Passenger passenger : current_passengers)
    {
        if (passenger.get_bus_stop() != current_stop.get_id())
        {
            aux.push_back(passenger);
        }
        else
        {
            passengers++;
        }
    }

    current_passengers = aux;

    return passengers;
}

int Bus::add_passengers(const int& simulation_time, BusStop &bus_stop)
{
    std::list<Passenger> aux;
    int passengers; 

    while (!bus_stop.get_passenger_list().empty())
    {
        Passenger first_passenger = bus_stop.pop_first_passenger();

        if (((simulation_time + time_in_bus_stop) < first_passenger.get_arrival_time()) || (current_passengers.size() >= max_capacity))
        {
            aux.push_back(first_passenger);
            break;
        }

        if ((first_passenger.get_arrival_time() + first_passenger.get_waiting_time()) < simulation_time) // Gone passengers
        {
            bus_stop.add_gone_passengers(1);
            continue;
        }

        if (true) //Aquí va la condición que determina si el pasajero va a una parada de la ruta del bus actual (Por ahora todos se suben)
        {
            passengers++;
            current_passengers.push_back(first_passenger);
            attended_passengers += 1;
        }
        else
        {
            aux.push_back(first_passenger);
        }
    }

    for (const auto &passenger : aux)
    {
        bus_stop.add_passenger(passenger);
    }

    return passengers;
}

int Bus::get_attended_passengers() const noexcept
{
    return attended_passengers;
}

void Bus::reset()
{
    current_passengers.clear();
    attended_passengers = 0;
}

void Bus::repair_bus(bool repair_engine, bool repair_breaks, bool repair_tires)
{
    if (repair_engine)
    {
        engine_state = 100;
    }
    if (repair_breaks)
    {
        breaks_state = 100;
    }
    if (repair_tires)
    {
        tires_state = 100;
    }
}

void Bus::calc_wear(int km)
{
    engine_state -= (km/20);
    breaks_state -= (km/10);
    tires_state -= (km/10);
    fuel -= (km/3);
}

std::vector<int> Bus::calc_maintenance_price()
{
    int engine_price = 0;
    int breaks_price = 0; 
    int tires_price = 0; 

    engine_price = (200 - engine_state)*(200 - engine_state);
    breaks_price = (100 - breaks_state)*(100 - breaks_state);
    tires_price = (100 - tires_state)*(100 - tires_price);

    if (engine_state <= 0)
    {
        engine_price = 40000;
    }
    if (breaks_state <= 0)
    {
        breaks_price = 20000;
    }
    if (tires_state <= 0)
    {
        tires_price = 20000;
    }

    return std::vector<int> {engine_price, breaks_price, tires_price};
}

bool Bus::get_in_route() const noexcept
{
    return in_route;
}

void Bus::set_in_route(bool _in_route)
{
    in_route = _in_route;
}