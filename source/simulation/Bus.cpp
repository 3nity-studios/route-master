#include <Bus.hpp>

Bus::Bus() : name(""), max_capacity(0), current_passengers(std::list<Passenger>{}), time_in_bus_stop(0)
{
    //empty
}

Bus::Bus(std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop) 
    : name(_name), max_capacity(_max_capacity), current_passengers(_current_passengers), time_in_bus_stop(_time_in_bus_stop)
{
    //empty
}

std::list<Passenger> Bus::get_passenger_list()
{
    return current_passengers;
}

void Bus::leave_passengers(int current_stop)
{
    std::list<Passenger> aux = current_passengers;
    current_passengers.clear();

    for (Passenger passenger : aux)
    {
        if (passenger.get_bus_stop() != current_stop)
        {
            current_passengers.push_back(passenger);
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