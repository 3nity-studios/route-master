#include "simulation/Bus.hpp"

Bus::Bus() : id(0), name(""), max_capacity(0), current_passengers(std::list<Passenger>{}), time_in_bus_stop(0), in_route(false)
{
    features["engine"] = {100, 500, 0, 1};
    features["breaks"] = {100, 200, 0, 1};
    features["tires"] = {100, 100, 0, 1};
    features["fuel"] = {100, 0, 0, 1};
}

Bus::Bus(int _id, std::string _name, int _max_capacity, std::list<Passenger> _current_passengers, int _time_in_bus_stop, 
         int engine_base_value, int engine_price, int breaks_base_value, int breaks_price, 
         int tires_base_value, int tires_price, int fuel_base_value, int fuel_price) 
    : id(_id), name(_name), max_capacity(_max_capacity), current_passengers(_current_passengers), time_in_bus_stop(_time_in_bus_stop), in_route(false)
{
    features["engine"] = {engine_base_value, engine_price, 0, 1};
    features["breaks"] = {breaks_base_value, breaks_price, 0, 1};
    features["tires"] = {tires_base_value, tires_price, 0, 1};
    features["fuel"] = {fuel_base_value, fuel_price, 0, 1};
}

Bus::Bus(nlohmann::json j)
{
    id = j["id"];
    name = j["name"];
    max_capacity = j["max_capacity"];
    for (auto &passenger : j["current_passengers"].items())
    {
        current_passengers.push_back(Passenger(passenger.value()));
    }
    time_in_bus_stop = j["time_in_bus_stop"];
    in_route = j["in_route"];

    for (auto &feature : j["features"].items())
    {
        BusFeature bus_feature;
        bus_feature.base_value = feature.value()["base_value"];
        bus_feature.base_price = feature.value()["base_price"];
        bus_feature.current_wear = feature.value()["current_wear"];
        bus_feature.current_level = feature.value()["current_level"];
        features[feature.key()] = bus_feature;
    }
}

int Bus::get_id() const noexcept
{
    return id;
}

void Bus::set_id(const int& _id)
{
    id = _id;
}

std::string Bus::get_name() const noexcept
{
    return name; 
}

int Bus::get_max_capacity() const noexcept
{
    return max_capacity;
}

std::list<Passenger> Bus::get_passenger_list() const noexcept
{
    return current_passengers;
}

BusFeature Bus::get_feature_info(const std::string name) const noexcept
{
    return features.at(name);
}

int Bus::get_time_in_bus_stop() const noexcept
{
    return time_in_bus_stop;
}

int Bus::get_engine_state() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("engine").current_wear) / (features.at("engine").base_value * LevelMultiplier[features.at("engine").current_level]));
}

int Bus::get_breaks_state() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("breaks").current_wear) / (features.at("breaks").base_value * LevelMultiplier[features.at("breaks").current_level]));
}

int Bus::get_tires_state() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("tires").current_wear) / (features.at("tires").base_value * LevelMultiplier[features.at("tires").current_level]));
}

int Bus::get_fuel() const noexcept
{
    return 100 * (1 - static_cast<float>(features.at("fuel").current_wear) / (features.at("fuel").base_value * LevelMultiplier[features.at("fuel").current_level]));
}

int Bus::leave_passengers(BusStop &current_stop)
{
    std::list<Passenger> aux;
    int passengers = 0; 

    for (Passenger passenger : current_passengers)
    {
        if (passenger.get_bus_stop() != current_stop.get_id())
        {
            aux.push_back(passenger);
        }
        else
        {
            passengers++;
        }
    }

    current_passengers = aux;

    return passengers;
}

int Bus::add_passengers(const float& simulation_time, BusStop &bus_stop)
{
    std::list<Passenger> aux;
    int passengers = 0; 

    while (!bus_stop.get_passenger_list().empty())
    {
        Passenger first_passenger = bus_stop.pop_first_passenger();

        if (((simulation_time + time_in_bus_stop/60.f) < first_passenger.get_arrival_time()) || (current_passengers.size() >= max_capacity))
        {
            aux.push_back(first_passenger);
            break;
        }

        if ((first_passenger.get_arrival_time() + first_passenger.get_waiting_time()/60.f) < simulation_time) // Gone passengers
        {
            bus_stop.add_gone_passengers(1);
            continue;
        }

        if (true) 
        {
            passengers++;
            current_passengers.push_back(first_passenger);
            attended_passengers += 1;
        }
        else
        {
            aux.push_back(first_passenger);
        }
    }

    for (const auto &passenger : aux)
    {
        bus_stop.add_passenger(passenger);
    }

    return passengers;
}

int Bus::get_attended_passengers() const noexcept
{
    return attended_passengers;
}

void Bus::reset()
{
    current_passengers.clear();
    attended_passengers = 0;
}

void Bus::repair_bus(bool repair_engine, bool repair_breaks, bool repair_tires, bool refuel)
{
    if (repair_engine)
    {
        features["engine"].current_wear = 0;
    }
    if (repair_breaks)
    {
        features["breaks"].current_wear = 0;
    }
    if (repair_tires)
    {
        features["tires"].current_wear = 0;
    }
    if (refuel)
    {
        features["fuel"].current_wear = 0;
    }
}

void Bus::improve_bus(bool improve_engine, bool improve_breaks, bool improve_tires, bool improve_fuel)
{
    if (improve_engine && features["engine"].current_level < 5)
    {
        features["engine"].current_level++;
    }
    if (improve_breaks && features["breaks"].current_level < 5)
    {
        features["breaks"].current_level++;
    }
    if (improve_tires && features["tires"].current_level < 5)
    {
        features["tires"].current_level++;
    }
    if (improve_fuel && features["fuel"].current_level < 5)
    {
        features["fuel"].current_level++;
    }
}

void Bus::calc_wear(int travelled_distance)
{
    features["engine"].current_wear += (travelled_distance / 200);
    features["breaks"].current_wear += (travelled_distance / 100);
    features["tires"].current_wear += (travelled_distance / 100);
    features["fuel"].current_wear += (travelled_distance / 30);
}

std::vector<int> Bus::calc_maintenance_price()
{
    int engine_price = (100 - get_engine_state())*(100 - get_engine_state());
    int breaks_price = (100 - get_breaks_state())*(100 - get_breaks_state());
    int tires_price = (100 - get_tires_state())*(100 - get_tires_state());
    int fuel_price = (100 - get_fuel())*(100 - get_fuel());

    if (features["engine"].current_wear >= features["engine"].base_value * features["engine"].current_level)
    {
        engine_price = 4000;
    }
    if (features["breaks"].current_wear >= features["breaks"].base_value * features["breaks"].current_level)
    {
        breaks_price = 800;
    }
    if (features["tires"].current_wear >= features["tires"].base_value * features["tires"].current_level)
    {
        tires_price = 400;
    }
    if (features["fuel"].current_wear >= features["fuel"].base_value * features["fuel"].current_level)
    {
        fuel_price = 80;
    }

    return std::vector<int> {engine_price, breaks_price, tires_price, fuel_price};
}

std::vector<int> Bus::calc_improvements_price()
{
    int engine_price = features["engine"].base_price * LevelMultiplier[features["engine"].current_level];
    int breaks_price = features["breaks"].base_price * LevelMultiplier[features["breaks"].current_level];
    int tires_price = features["tires"].base_price * LevelMultiplier[features["tires"].current_level];
    int fuel_price = features["fuel"].base_price * LevelMultiplier[features["fuel"].current_level];

    return std::vector<int> {engine_price, breaks_price, tires_price, fuel_price};
}

bool Bus::get_in_route() const noexcept
{
    return in_route;
}

void Bus::set_in_route(bool _in_route)
{
    in_route = _in_route;
}

nlohmann::json Bus::to_json()
{
    nlohmann::json bus_json;

    bus_json["id"] = id;
    bus_json["name"] = name;
    bus_json["max_capacity"] = max_capacity;
    for(auto &passenger : current_passengers)
    {
        bus_json["current_passengers"].push_back(passenger.to_json());
    }
    bus_json["time_in_bus_stop"] = time_in_bus_stop;
    bus_json["in_route"] = in_route;

    nlohmann::json features_json;
    for (auto &feature : features)
    {
        nlohmann::json feature_json;
        feature_json["base_value"] = feature.second.base_value;
        feature_json["base_price"] = feature.second.base_price;
        feature_json["current_wear"] = feature.second.current_wear;
        feature_json["current_level"] = feature.second.current_level;
        features_json[feature.first] = feature_json;
    }
    bus_json["features"] = features_json;

    return bus_json;
}