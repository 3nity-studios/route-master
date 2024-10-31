#pragma once

#include <string>

class Employee
{
    private:

    int id;
    std::string name; 
    std::string last_name;
    ushort age;
    ushort shift_len;
    ushort fatigue; 
    int accumulated_work_time;
    int hourly_rate;

    public:

    Employee();
    Employee(int _id, std::string _name, std::string _last_name, ushort _age, int _hourly_rate, ushort _shift_len, ushort _fatigue); 

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    std::string get_last_name() const noexcept;
    ushort get_age() const noexcept;
    ushort get_shift_len() const noexcept;
    ushort get_fatigue() const noexcept;
    int get_accumulated_work_time()const noexcept;
    int get_hourly_rate() const noexcept;

    void set_id(const int& _id);
    void set_name(const std::string& _name);
    void set_last_name(const std::string& _last_name);
    void set_age(const ushort& _age);
    void set_shift_len(const ushort& _shift_len);
    void set_fatigue(const ushort& _fatigue);
    void set_accumulated_work_time(const int& time);
    void set_hourly_rate(const int& rate);

    void calc_fatigue(int km);
};