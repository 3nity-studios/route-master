#pragma once

#include <Designar/list.hpp>
#include <nlohmann/json.hpp>
#include "player/Item.hpp"
#include "player/Player.hpp"
#include "player/Achievement.hpp"

class Store
{
    private:
    Designar::SLList<Item> inventory; 
    Designar::SLList<Bus> bus_list;
    Designar::SLList<Employee> employee_list; 

    public: 

    Store();
    Store(Designar::SLList<Item> _inventory, Designar::SLList<Bus> _bus_list, Designar::SLList<Employee> _employee_list);
    Store(nlohmann::json j);

    Designar::SLList<Item> get_inventory();
    Designar::SLList<Bus> get_bus_list();
    Designar::SLList<Employee> get_employee_list();

    void set_inventory(Designar::SLList<Item> _inventory);
    void set_bus_list(Designar::SLList<Bus> _bus_list);
    void set_employee_list(Designar::SLList<Employee> _employee_list);

    void add_bus_to_inventory(Bus bus, int price, int amount);
    void add_employee_to_inventory(Employee employee, int price, int amount);

    void buy_item(Player &player, int item_id, int amount);
    void buy_bus_maintenance(int id, Player &player, bool repair_engine, bool repair_breaks, bool repair_tires, bool refuel);
    void buy_bus_improvements(int id, Player &player, bool improve_engine, bool improve_breaks, bool improve_tires, bool improve_fuel);
    void pay_employee(int id, Player &player);

    void claim_achievement(Player &player, Achievement &achievement);
    void pay_demand(Player &player, int amount);
    void pay_tow(Player &player, int id);

    nlohmann::json to_json();
    void save();
};