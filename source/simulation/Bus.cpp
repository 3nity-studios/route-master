#include <Bus.hpp>

Bus::Bus() : name(""), max_capacity(0), current_passengers(std::list<Passenger>{}), time_in_bus_stop(0), engine_state(100), breaks_state(100), tires_state(100), fuel(100)
{
    //empty
}

Bus::Bus(std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop) 
    : name(_name), max_capacity(_max_capacity), current_passengers(_current_passengers), time_in_bus_stop(_time_in_bus_stop), engine_state(100), breaks_state(100), tires_state(100), fuel(100)
{
    //empty
}

std::string Bus::get_name()
{
    return name; 
}

int Bus::get_max_capacity()
{
    return max_capacity;
}

std::list<Passenger> Bus::get_passenger_list()
{
    return current_passengers;
}

int Bus::get_time_in_bus_stop()
{
    return time_in_bus_stop;
}

int Bus::get_engine_state()
{
    return engine_state;
}

int Bus::get_breaks_state()
{
    return breaks_state;
}

int Bus::get_tires_state()
{
    return tires_state;
}

int Bus::get_fuel()
{
    return fuel;
}

void Bus::leave_passengers(BusStop &current_stop)
{
    std::list<Passenger> aux = current_passengers;
    current_passengers.clear();

    for (Passenger passenger : aux)
    {
        if (passenger.get_bus_stop() != current_stop.get_id())
        {
            current_passengers.push_back(passenger);
        }
        else
        {
            current_stop.add_passenger(passenger);
        }
    }
}

void Bus::add_passengers(int simulation_time, BusStop &bus_stop)
{
    std::list<Passenger> aux;

    while (!bus_stop.get_passenger_list().empty())
    {
        Passenger first_passenger = bus_stop.get_passenger_list().top();

        if (((simulation_time + time_in_bus_stop) < first_passenger.get_arrival_time()) || (current_passengers.size() >= max_capacity))
        {
            break;
        }

        if ((first_passenger.get_arrival_time() + first_passenger.get_waiting_time()) < simulation_time)
        {
            aux.push_back(bus_stop.pop_first_passenger());
            continue;
        }

        if (true) //Aquí va la condición que determina si el pasajero va a una parada de la ruta del bus actual (Por ahora todos se suben)
        {
            current_passengers.push_back(bus_stop.pop_first_passenger());
        }
        else
        {
            aux.push_back(bus_stop.pop_first_passenger());
        }
    }

    for (const auto &passenger : aux)
    {
        bus_stop.add_passenger(passenger);
    }
}

void Bus::calc_wear(int km)
{
    engine_state -= (km/20);
    breaks_state -= (km/10);
    tires_state -= (km/10);
    fuel -= (km/3);
}