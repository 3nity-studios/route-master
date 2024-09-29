#include <Employee.hpp>

Employee::Employee() : name(""), last_name(""), age(0), shift_len(0), fatigue(0)
{
    //empty
}

Employee::Employee(std::string _name, std::string _last_name, ushort _age, ushort _shift_len, ushort _fatigue) 
    : name(_name), last_name(_last_name), age(_age), shift_len(_shift_len), fatigue(_fatigue)
{
    //empty
}

std::string Employee::get_name()
{
    return name; 
}

std::string Employee::get_last_name()
{
    return last_name; 
}

ushort Employee::get_age()
{
    return age; 
}

ushort Employee::get_shift_len()
{
    return shift_len; 
}

ushort Employee::get_fatigue()
{
    return fatigue; 
}

void Employee::set_name(std::string _name)
{
    name = _name; 
}

void Employee::set_last_name(std::string _last_name)
{
    last_name = _last_name; 
}

void Employee::set_age(ushort _age)
{
    age = _age; 
}

void Employee::set_shift_len(ushort _shift_len)
{
    shift_len = _shift_len; 
}

void Employee::set_fatigue(ushort _fatigue)
{
    fatigue = _fatigue; 
}

void Employee::calc_fatigue(int km)
{
    fatigue = fatigue + (km/2);
}