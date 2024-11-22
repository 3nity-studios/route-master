#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Employee
{
    private:

    int id;
    std::string name; 
    std::string last_name;
    unsigned short age;
    unsigned short shift_len;
    unsigned short fatigue; 
    float total_work_hours;
    int hourly_rate;
    bool in_route; 

    public:

    Employee();
    Employee(int _id, std::string _name, std::string _last_name, unsigned short _age, int _hourly_rate, unsigned short _shift_len, unsigned short _fatigue); 
    Employee(nlohmann::json j);

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    std::string get_last_name() const noexcept;
    unsigned short get_age() const noexcept;
    unsigned short get_shift_len() const noexcept;
    unsigned short get_fatigue() const noexcept;
    float get_total_work_hours()const noexcept;
    int get_hourly_rate() const noexcept;
    bool get_in_route() const noexcept; 

    void set_id(const int& _id);
    void set_name(const std::string& _name);
    void set_last_name(const std::string& _last_name);
    void set_age(const unsigned short& _age);
    void set_shift_len(const unsigned short& _shift_len);
    void set_fatigue(const unsigned short& _fatigue);
    void set_total_work_hours(const float& time);
    void set_hourly_rate(const int& rate);
    void set_in_route(const bool& _in_route);

    void calc_fatigue(int distance, int time);

    nlohmann::json to_json();
};