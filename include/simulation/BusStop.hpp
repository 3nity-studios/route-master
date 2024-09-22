#include <Passenger.hpp>
#include <queue>
#include <string>

struct Cmp
{
    bool operator () (Passenger a, Passenger b)
    {
        return a.get_arrival_time() > b.get_arrival_time();
    }
};

using PassengerHeap = std::priority_queue<Passenger, std::vector<Passenger>, Cmp>;

class BusStop
{
    private:

    int id;
    std::string name; 
    PassengerHeap passenger_list;
    float avg_arrival_time;
    float avg_waiting_time;
    float sd_waiting_time;
    float avg_bus_stop;
    float sd_bus_stop;

    public: 

    BusStop();
    BusStop(int _id, std::string _name, float _avg_arrival_time, float _avg_waiting_time, float _sd_waiting_time, float _avg_bus_stop, float _sd_bus_stop);

    std::string get_name();
    PassengerHeap get_passenger_list();

    void set_name(std::string _name);
    void set_passenger_list(PassengerHeap _passenger_list);

    void generate_passengers();

    void add_passenger(Passenger passenger);
    Passenger pop_first_passenger();
};