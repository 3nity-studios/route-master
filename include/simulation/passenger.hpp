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

    int get_arrival_time() const noexcept;
    int get_waiting_time() const noexcept;
    int get_bus_stop() const noexcept;
    bool is_gone(int current_time) const noexcept;

    void set_arrival_time(const int& _arrival_time);
    void set_waiting_time(const int& _waiting_time);
    void set_bus_stop(const int& _bus_stop);
};
