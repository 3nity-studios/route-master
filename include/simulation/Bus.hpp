#pragma once

#include <queue>
#include <list>
#include <string>
#include "simulation/BusStop.hpp"

class Bus
{
    private:

    int id;
    std::string name; 
    int max_capacity;
    std::list<Passenger> current_passengers;
    int attended_passengers;
    int time_in_bus_stop;
    int engine_state;
    int breaks_state;
    int tires_state;
    int fuel; 

    public: 
    Bus();
    Bus(int _id, std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop);

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    int get_max_capacity() const noexcept;
    std::list<Passenger> get_passenger_list() const noexcept;
    int get_time_in_bus_stop() const noexcept;
    int get_engine_state() const noexcept;
    int get_breaks_state() const noexcept;
    int get_tires_state() const noexcept;
    int get_fuel() const noexcept; 

    void leave_passengers(BusStop &current_stop);
    void add_passengers(const int& simulation_time, BusStop &bus_stop);
    int get_attended_passengers() const noexcept;
    void reset();
    void repair_bus(bool repair_engine, bool repair_breaks, bool repair_tires);

    void calc_wear(int km);
    std::vector <int> calc_maintenance_price();
};;
