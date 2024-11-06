#pragma once

#include <list>
#include <map>
#include <queue>
#include <string>
#include "simulation/BusStop.hpp"

static std::map<unsigned int, float> LevelMultiplier = {{1, 1.0}, {2, 1.25}, {3, 1.5}, {4, 2.0}, {5, 3.0}};

struct BusFeature
{
    int base_value;
    int base_price;
    int current_wear;
    unsigned int current_level;
};

class Bus
{
  private:
    int id;
    std::string name;
    int max_capacity;
    std::list<Passenger> current_passengers;
    int attended_passengers;
    int time_in_bus_stop;
    std::map<std::string, BusFeature> features;

  public:
    Bus();
    Bus(int _id, std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop,
        int engine_base_value, int engine_price, int breaks_base_value, int breaks_price, int tires_base_value,
        int tires_price, int fuel_base_value, int fuel_price);

    int get_id() const noexcept;
    void set_id(const int &_id);
    std::string get_name() const noexcept;
    int get_max_capacity() const noexcept;
    std::list<Passenger> get_passenger_list() const noexcept;
    BusFeature get_feature_info(const std::string name) const noexcept;
    int get_time_in_bus_stop() const noexcept;
    int get_engine_state() const noexcept;
    int get_breaks_state() const noexcept;
    int get_tires_state() const noexcept;
    int get_fuel() const noexcept;

    int leave_passengers(BusStop &current_stop);
    int add_passengers(const int &simulation_time, BusStop &bus_stop);
    int get_attended_passengers() const noexcept;
    void reset();
    void repair_bus(bool repair_engine, bool repair_breaks, bool repair_tires, bool refuel);
    void improve_bus(bool improve_engine, bool improve_breaks, bool improve_tires, bool improve_fuel);

    void calc_wear(int travelled_distance);
    std::vector<int> calc_maintenance_price();
    std::vector<int> calc_improvements_price();
};
