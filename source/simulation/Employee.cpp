#include "simulation/Employee.hpp"

Employee::Employee() : id(0), name(""), last_name(""), age(0), shift_len(0), fatigue(0)
{
    //empty
}

Employee::Employee(int _id, std::string _name, std::string _last_name, ushort _age, ushort _shift_len, ushort _fatigue) 
    : id(_id), name(_name), last_name(_last_name), age(_age), shift_len(_shift_len), fatigue(_fatigue)
{
    //empty
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

void Employee::calc_fatigue(int km)
{
    fatigue = fatigue + (km/2);
}
