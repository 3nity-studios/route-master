#include "simulation/Employee.hpp"

Employee::Employee() : id(0), name(""), last_name(""), age(0), shift_len(0), fatigue(0), in_route(false)
{
    //empty
}

Employee::Employee(int _id, std::string _name, std::string _last_name, ushort _age, int _hourly_rate, ushort _shift_len, ushort _fatigue) 
    : id(_id), name(_name), last_name(_last_name), age(_age), hourly_rate(_hourly_rate), total_work_hours(0), shift_len(_shift_len), fatigue(_fatigue), in_route(false)
{
    //empty
}

Employee::Employee(nlohmann::json j)
{
    id = j["id"];
    name = j["name"];
    last_name = j["last_name"];
    age = j["age"];
    shift_len = j["shift_len"];
    fatigue = j["fatigue"];
    total_work_hours = j["total_work_hours"];
    hourly_rate = j["hourly_rate"];
    in_route = j["in_route"];
}

int Employee::get_id() const noexcept
{
    return id;
}

std::string Employee::get_name() const noexcept
{
    return name; 
}

std::string Employee::get_last_name() const noexcept
{
    return last_name; 
}

ushort Employee::get_age() const noexcept
{
    return age; 
}

ushort Employee::get_shift_len() const noexcept
{
    return shift_len; 
}

ushort Employee::get_fatigue() const noexcept
{
    return fatigue; 
}

int Employee::get_total_work_hours() const noexcept
{
    return total_work_hours;
}

int Employee::get_hourly_rate() const noexcept
{
    return hourly_rate;
}

bool Employee::get_in_route() const noexcept
{
    return in_route;
} 

void Employee::set_id(const int& _id)
{
    id = _id; 
}

void Employee::set_name(const std::string& _name)
{
    name = _name; 
}

void Employee::set_last_name(const std::string& _last_name)
{
    last_name = _last_name; 
}

void Employee::set_age(const ushort& _age)
{
    age = _age; 
}

void Employee::set_shift_len(const ushort& _shift_len)
{
    shift_len = _shift_len; 
}

void Employee::set_fatigue(const ushort& _fatigue)
{
    fatigue = _fatigue; 
}

void Employee::set_total_work_hours(const int& time)
{
    total_work_hours = time;
}

void Employee::set_hourly_rate(const int& rate)
{
    hourly_rate = rate;
}

void Employee::calc_fatigue(int km)
{
    fatigue = fatigue + (km/2);
}

void Employee::set_in_route(const bool& _in_route)
{
    in_route = _in_route;
}

nlohmann::json Employee::to_json()
{
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["last_name"] = last_name;
    j["age"] = age;
    j["shift_len"] = shift_len;
    j["fatigue"] = fatigue;
    j["total_work_hours"] = total_work_hours;
    j["hourly_rate"] = hourly_rate;
    j["in_route"] = in_route;
    return j;
}