#pragma once

#include <string>
#include <Designar/tree.hpp>
#include <nlohmann/json.hpp>
#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"

struct Cmp2
{
    bool operator () (Employee a, Employee b)
    {
        return a.get_id() < b.get_id();
    }

    bool operator () (Bus a, Bus b)
    {
        return a.get_id() < b.get_id();
    }
};

class Player
{
    private:
    int id;
    std::string name;
    int balance;
    Designar::RankedTreap<Bus, Cmp2> buses;
    Designar::RankedTreap<Employee, Cmp2> employees;

    public:
    Player();
    Player(int _id, const std::string& _name, int _balance);
    Player(nlohmann::json j);

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    Designar::SLList<Bus> get_buses();
    Designar::SLList<Employee> get_employees(); 
    Bus &get_bus(const int& id);
    Employee &get_employee(const int& id);
    int get_balance();

    void set_name(const std::string& name);
    void add_bus(Bus bus);
    void add_employee(Employee employee);
    void increase_balance(int balance_to_increase);
    void decrease_balance(int balance_to_decrease);
    nlohmann::json to_json();
};