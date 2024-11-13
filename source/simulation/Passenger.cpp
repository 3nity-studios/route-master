#include "simulation/Passenger.hpp"

Passenger::Passenger() : arrival_time(0), waiting_time(0), bus_stop(0)
{
    //empty
}

Passenger::Passenger(float _arrival_time, float _waiting_time, int _bus_stop) 
    : arrival_time(_arrival_time), waiting_time(_waiting_time), bus_stop(_bus_stop)
{
    //empty
}

Passenger::Passenger(nlohmann::json j)
{
    arrival_time = j["arrival_time"];
    waiting_time = j["waiting_time"];
    bus_stop = j["bus_stop"];
}

bool Passenger::is_gone(float current_time) const noexcept
{
    return arrival_time + waiting_time < current_time;
}

float Passenger::get_arrival_time() const noexcept
{
    return arrival_time;
}

float Passenger::get_waiting_time() const noexcept
{
    return waiting_time;
}

int Passenger::get_bus_stop() const noexcept
{
    return bus_stop;
}

void Passenger::set_arrival_time(const float& _arrival_time)
{
    arrival_time = _arrival_time; 
}

void Passenger::set_waiting_time(const float& _waiting_time)
{
    waiting_time = _waiting_time;
}

void Passenger::set_bus_stop(const int& _bus_stop)
{
    bus_stop = _bus_stop;
}

nlohmann::json Passenger::to_json()
{
    nlohmann::json j;

    j["arrival_time"] = arrival_time;
    j["waiting_time"] = waiting_time;
    j["bus_stop"] = bus_stop;

    return j;
}
