#include <BusStop.hpp>
#include <random>
#include <ctime>

BusStop::BusStop()
    : id(0), name(""), avg_arrival_time(0.0), avg_waiting_time(0.0), sd_waiting_time(0.0), avg_bus_stop(0.0), sd_bus_stop(0.0)
{
    // empty
}

BusStop::BusStop(int _id, std::string _name, float _avg_arrival_time, float _avg_waiting_time, float _sd_waiting_time, float _avg_bus_stop, float _sd_bus_stop)
    :id(_id), name(_name), avg_arrival_time(_avg_arrival_time), avg_waiting_time(_avg_waiting_time), sd_waiting_time(_sd_waiting_time), avg_bus_stop(_avg_bus_stop), sd_bus_stop(_sd_bus_stop)
{
    // empty
}

int BusStop::get_id() const noexcept
{
    return id;
}

std::string BusStop::get_name() const noexcept
{
    return name;
}

void BusStop::set_name(const std::string& _name)
{
    name = _name;
}

PassengerHeap BusStop::get_passenger_list() const noexcept
{
    return passenger_list;
}

void BusStop::set_passenger_list(const PassengerHeap& _passenger_list)
{
    passenger_list = _passenger_list;
}

void BusStop::generate_passengers()
{
    gone_passengers = 0;
    while(!passenger_list.empty())
    {
        passenger_list.pop();
    }

    // Seed with a real random value, if available
    std::random_device rd;

    // Initialize random number generator
    std::mt19937 gen(rd());

    // Define distributions
    std::exponential_distribution<> arrival_time_dist(1.0 / avg_arrival_time); // Exponential distribution
    std::normal_distribution<> waiting_time_dist(avg_waiting_time, sd_waiting_time);
    std::normal_distribution<> bus_stop_dist(avg_bus_stop, sd_bus_stop);

    // Generate a random number of passengers
    int num_passengers = std::round(waiting_time_dist(gen));

    // Ensure the number of passengers is non-negative
    num_passengers = std::max(0, num_passengers);

    for (int i = 0; i < num_passengers; ++i)
    {
        // Generate random values for passenger attributes
        int arrival_time = std::round(arrival_time_dist(gen));
        int waiting_time = std::round(waiting_time_dist(gen));
        int bus_stop = std::round(bus_stop_dist(gen));

        // Create a new passenger with generated values
        Passenger passenger(arrival_time, waiting_time, bus_stop);

        // Add the passenger to the passenger list
        passenger_list.push(passenger);
    }
}

void BusStop::add_passenger(const Passenger& passenger)
{
    passenger_list.push(passenger);
}

Passenger BusStop::pop_first_passenger()
{
    Passenger aux = passenger_list.top();
    passenger_list.pop();

    return aux;
}

int BusStop::get_gone_passengers() const noexcept
{
    return gone_passengers;
}

void BusStop::add_gone_passengers(const int& num)
{
    gone_passengers += num;
}