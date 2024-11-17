#include "simulation/Employee.hpp"

Employee::Employee() : id(0), name(""), last_name(""), age(0), shift_len(0), fatigue(0), in_route(false)
{
    //empty
}

Employee::Employee(int _id, std::string _name, std::string _last_name, unsigned short _age, int _hourly_rate, unsigned short _shift_len, unsigned short _fatigue) 
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

unsigned short Employee::get_age() const noexcept
{
    return age; 
}

unsigned short Employee::get_shift_len() const noexcept
{
    return shift_len; 
}

unsigned short Employee::get_fatigue() const noexcept
{
    return fatigue; 
}

float Employee::get_total_work_hours() const noexcept
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

void Employee::set_age(const unsigned short& _age)
{
    age = _age; 
}

void Employee::set_shift_len(const unsigned short& _shift_len)
{
    shift_len = _shift_len; 
}

void Employee::set_fatigue(const unsigned short& _fatigue)
{
    fatigue = _fatigue; 
}

void Employee::set_total_work_hours(const float& time)
{
    total_work_hours = time;
}

void Employee::set_hourly_rate(const int& rate)
{
    hourly_rate = rate;
}

void Employee::calc_fatigue(int distance, int time)
{
    fatigue += distance / 2;
    total_work_hours += time / 60.0f;
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