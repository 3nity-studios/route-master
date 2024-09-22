#include <Passenger.hpp>

Passenger::Passenger() : arrival_time(0), waiting_time(0), bus_stop(0)
{
    //empty
}

Passenger::Passenger(int _arrival_time, int _waiting_time, int _bus_stop) 
    : arrival_time(_arrival_time), waiting_time(_waiting_time), bus_stop(_bus_stop)
{
    //empty
}

int Passenger::get_arrival_time()
{
    return arrival_time;
}

int Passenger::get_waiting_time()
{
    return waiting_time;
}

int Passenger::get_bus_stop()
{
    return bus_stop;
}

void Passenger::set_arrival_time(int _arrival_time)
{
    arrival_time = _arrival_time; 
}

void Passenger::set_waiting_time(int _waiting_time)
{
    waiting_time = _waiting_time;
}

void Passenger::set_bus_stop(int _bus_stop)
{
    bus_stop = _bus_stop;
}
