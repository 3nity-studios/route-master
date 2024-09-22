#include <string>

class Passenger
{
    private:

    int arrival_time;
    int waiting_time;
    int bus_stop;

    public:

    Passenger();
    Passenger(int _arrival_time, int _waiting_time, int _bus_stop);

    int get_arrival_time();
    int get_waiting_time();
    int get_bus_stop();

    void set_arrival_time(int _arrival_time);
    void set_waiting_time(int _waiting_time);
    void set_bus_stop(int _bus_stop);
};
