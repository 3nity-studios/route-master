#include "simulation/Bus.hpp"

Bus::Bus() : id(0), name(""), max_capacity(0), current_passengers(std::list<Passenger>{}), time_in_bus_stop(0)
{
    features["engine"] = {100, 500, 0, 1};
    features["breaks"] = {100, 200, 0, 1};
    features["tires"] = {100, 100, 0, 1};
    features["fuel"] = {100, 0, 0, 1};
}

Bus::Bus(int _id, std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop, 
         int engine_base_value, int engine_price, int breaks_base_value, int breaks_price, 
         int tires_base_value, int tires_price, int fuel_base_value, int fuel_price) 
    : id(_id), name(_name), max_capacity(_max_capacity), current_passengers(_current_passengers), time_in_bus_stop(_time_in_bus_stop)
{
    features["engine"] = {engine_base_value, engine_price, 0, 1};
    features["breaks"] = {breaks_base_value, breaks_price, 0, 1};
    features["tires"] = {tires_base_value, tires_price, 0, 1};
    features["fuel"] = {fuel_base_value, fuel_price, 0, 1};
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
    return 100 * (1 - static_cast<float>(features.at("engine").current_wear) / (features.at("engine").base_value * features.at("engine").current_level));
}

int Bus::get_breaks_state() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("breaks").current_wear) / (features.at("breaks").base_value * features.at("breaks").current_level));
}

int Bus::get_tires_state() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("tires").current_wear) / (features.at("tires").base_value * features.at("tires").current_level));
}

int Bus::get_fuel() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("fuel").current_wear) / (features.at("fuel").base_value * features.at("fuel").current_level));
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
    int passengers = 0; 

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

void Bus::repair_bus(bool repair_engine, bool repair_breaks, bool repair_tires, bool refuel)
{
    if (repair_engine)
    {
        features["engine"].current_wear = 0;
    }
    if (repair_breaks)
    {
        features["breaks"].current_wear = 0;
    }
    if (repair_tires)
    {
        features["tires"].current_wear = 0;
    }
    if (refuel)
    {
        features["fuel"].current_wear = 0;
    }
}

void Bus::calc_wear(int travelled_distance)
{
    features["engine"].current_wear += (travelled_distance / 20);
    features["breaks"].current_wear += (travelled_distance / 10);
    features["tires"].current_wear += (travelled_distance / 10);
    features["fuel"].current_wear += (travelled_distance / 3);
}

std::vector<int> Bus::calc_maintenance_price()
{
    int engine_price = (100 - get_engine_state())*(100 - get_engine_state());
    int breaks_price = (100 - get_breaks_state())*(100 - get_breaks_state());
    int tires_price = (100 - get_tires_state())*(100 - get_tires_state());
    int fuel_price = (100 - get_fuel())*(100 - get_fuel());

    if (features["engine"].current_wear >= features["engine"].base_value * features["engine"].current_level)
    {
        engine_price = 4000;
    }
    if (features["breaks"].current_wear >= features["breaks"].base_value * features["breaks"].current_level)
    {
        breaks_price = 2000;
    }
    if (features["tires"].current_wear >= features["tires"].base_value * features["tires"].current_level)
    {
        tires_price = 2000;
    }
    if (features["fuel"].current_wear >= features["fuel"].base_value * features["fuel"].current_level)
    {
        fuel_price = 1000;
    }

    return std::vector<int> {engine_price, breaks_price, tires_price, fuel_price};
}