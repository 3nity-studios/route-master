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

    public: 

    Bus();
    Bus(std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop);

    std::list<Passenger> get_passenger_list();

    void leave_passengers(int current_stop);
    void add_passengers(int simulation_time, BusStop &bus_stop);
};