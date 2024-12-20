#include "simulation/BusStop.hpp"
#include <random>
#include <ctime>

BusStop::BusStop()
    : VisualElement(), name(""), avg_hourly_arrivals(24, 0), avg_arrival_time(0.0), avg_waiting_time(0.0), sd_waiting_time(0.0), avg_bus_stop(0.0), sd_bus_stop(0.0)
{
    // empty
}

BusStop::BusStop(int _id, std::string _name, std::vector<int> _avg_hourly_arrivals, float _avg_arrival_time, float _avg_waiting_time, float _sd_waiting_time, float _avg_bus_stop, float _sd_bus_stop, float _x, float _y)
    :VisualElement(_id, _x, _y), name(_name), avg_hourly_arrivals(_avg_hourly_arrivals), avg_arrival_time(_avg_arrival_time), avg_waiting_time(_avg_waiting_time), sd_waiting_time(_sd_waiting_time), avg_bus_stop(_avg_bus_stop), sd_bus_stop(_sd_bus_stop)
{
    // empty
}

BusStop::BusStop(nlohmann::json j)
    : VisualElement(j)
{
    name = j["name"];
    for (auto& passenger : j["passenger_list"])
    {
        passenger_list.push(Passenger(passenger));
    }
    for(auto avg : j["avg_hourly_arrivals"])
    {
        avg_hourly_arrivals.push_back(avg);
    }
    avg_arrival_time = j["avg_arrival_time"];
    avg_waiting_time = j["avg_waiting_time"];
    sd_waiting_time = j["sd_waiting_time"];
    avg_bus_stop = j["avg_bus_stop"];
    sd_bus_stop = j["sd_bus_stop"];
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

void BusStop::update(float current_time)
{
    if(!passenger_list.empty())
    {
        while(passenger_list.top().is_gone(current_time))
        {
            passenger_list.pop();
            gone_passengers++;
        }
    }
}

void BusStop::generate_passengers(int current_time_day)
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

    std::normal_distribution<> arrival_time_dist(avg_arrival_time, 30.f); // Exponential distribution
    std::normal_distribution<> waiting_time_dist(avg_waiting_time, sd_waiting_time);
    std::normal_distribution<> bus_stop_dist(avg_bus_stop, sd_bus_stop);

    for(int i = 0; i < avg_hourly_arrivals.size(); i++)
    {
        std::poisson_distribution<> passengers_per_hour(avg_hourly_arrivals[i]);
        int num_passengers = passengers_per_hour(gen);

        for(int j = 0; j < num_passengers; j++)
        {
            // Generate random values for passenger attributes
            float arrival_time = 24*current_time_day + i + arrival_time_dist(gen)/60.f;
            float waiting_time = waiting_time_dist(gen)/60.f;
            int bus_stop = std::round(bus_stop_dist(gen));

            // Create a new passenger with generated values
            Passenger passenger(arrival_time, waiting_time, bus_stop);

            // Add the passenger to the passenger list
            passenger_list.push(passenger);
        }
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

int BusStop::get_actual_passengers(float current_time)
{
    int passengers = 0; 
    auto aux = passenger_list;

    while (!aux.empty())
    {
        auto passenger = aux.top();

        if (passenger.get_arrival_time() <= current_time)
        {
            passengers++;
        }

        aux.pop();
    }

    return passengers;
}

nlohmann::json BusStop::to_json()
{
    nlohmann::json j = VisualElement::to_json();

    j["name"] = name;

    // Add passenger list
    while(!passenger_list.empty())
    {
        Passenger passenger = passenger_list.top();
        j["passenger_list"].push_back(passenger.to_json());
        passenger_list.pop();
    }
    for(auto avg : avg_hourly_arrivals)
    {
        j["avg_hourly_arrivals"].push_back(avg);
    }
    j["avg_arrival_time"] = avg_arrival_time;
    j["avg_waiting_time"] = avg_waiting_time;
    j["sd_waiting_time"] = sd_waiting_time;
    j["avg_bus_stop"] = avg_bus_stop;
    j["sd_bus_stop"] = sd_bus_stop;

    return j;
}