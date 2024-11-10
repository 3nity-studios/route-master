#pragma once

#include "simulation/Passenger.hpp"
#include "simulation/VisualElement.hpp"
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <vector>

struct Cmp
{
    bool operator () (Passenger a, Passenger b)
    {
        return a.get_arrival_time() > b.get_arrival_time();
    }
};

using PassengerHeap = std::priority_queue<Passenger, std::vector<Passenger>, Cmp>;

class BusStop : public VisualElement
{
    private:

    std::string name; 
    PassengerHeap passenger_list;
    int gone_passengers;
    std::vector<int> avg_hourly_arrivals;
    float avg_arrival_time;
    float avg_waiting_time;
    float sd_waiting_time;
    float avg_bus_stop;
    float sd_bus_stop;

    public: 

    BusStop();
    BusStop(int _id, std::string _name, std::vector<int> _avg_hourly_arrivals, float _avg_arrival_time, float _avg_waiting_time, float _sd_waiting_time, float _avg_bus_stop, float _sd_bus_stop, float _x, float _y);
    BusStop(nlohmann::json j);

    std::string get_name() const noexcept;
    void set_name(const std::string& _name);

    PassengerHeap get_passenger_list() const noexcept;
    void set_passenger_list(const PassengerHeap& _passenger_list);

    void update(float current_time);
    void generate_passengers();
    void add_passenger(const Passenger& passenger);
    Passenger pop_first_passenger();
    int get_actual_passengers(float current_time);

    int get_gone_passengers() const noexcept;
    void add_gone_passengers(const int& num);

    nlohmann::json to_json();

    bool operator==(const BusStop& other) const {
        return this->get_id() == other.get_id();
    }
};