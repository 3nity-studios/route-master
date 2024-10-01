#pragma once

#include <string>
#include <list>
#include <queue>
#include <BusStop.hpp>

class Bus
{
    private:

    std::string name; 
    int max_capacity;
    std::list<Passenger> current_passengers;
    int time_in_bus_stop;
    int engine_state;
    int breaks_state;
    int tires_state;
    int fuel; 

    public: 

    Bus();
    Bus(std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop);

    std::string get_name();
    int get_max_capacity();
    std::list<Passenger> get_passenger_list();
    int get_time_in_bus_stop();
    int get_engine_state();
    int get_breaks_state();
    int get_tires_state();
    int get_fuel(); 

    void leave_passengers(BusStop &current_stop);
    void add_passengers(int simulation_time, BusStop &bus_stop);

    void calc_wear(int km);
};