#include "player/Store.hpp"
#include <iostream>

Store::Store() : inventory(Designar::SLList<Item>{}), bus_list(Designar::SLList<Bus>{}), employee_list(Designar::SLList<Employee>{})
{
    // empty
}

Store::Store(Designar::SLList<Item> _inventory, Designar::SLList<Bus> _bus_list,
             Designar::SLList<Employee> _employee_list) : inventory(_inventory), bus_list(_bus_list), employee_list(_employee_list)
{
    // empty
}

Store::Store(nlohmann::json j)
{
    for (auto &item : j["inventory"])
    {
        inventory.append(Item(item));
    }

    for (auto &bus : j["bus_list"])
    {
        bus_list.append(Bus(bus));
    }

    for (auto &employee : j["employee_list"])
    {
        employee_list.append(Employee(employee));
    }
}

void Store::add_bus_to_inventory(Bus bus, int price, int amount)
{
    bus.set_id(inventory.size());
    inventory.append(Item(bus.get_id(), bus.get_name(), price, amount, ItemType::Bus, false));
    bus_list.append(bus);
}

void Store::add_employee_to_inventory(Employee employee, int price, int amount)
{
    employee.set_id(inventory.size());
    inventory.append(Item(employee.get_id(), employee.get_name(), price, amount, ItemType::Employee, false));
    employee_list.append(employee);
}

Designar::SLList<Item> Store::get_inventory()
{
    return inventory;
}

Designar::SLList<Bus> Store::get_bus_list()
{
    return bus_list;
}

Designar::SLList<Employee> Store::get_employee_list()
{
    return employee_list;
}

void Store::set_inventory(Designar::SLList<Item> _inventory)
{
    inventory = _inventory;
}

void Store::set_bus_list(Designar::SLList<Bus> _bus_list)
{
    bus_list = _bus_list;
}

void Store::set_employee_list(Designar::SLList<Employee> _employee_list)
{
    employee_list = _employee_list;
}

void Store::buy_item(Player &player, int item_id, int amount)
{
    for (auto &item : inventory)
    {
        if (item.get_id() == item_id)
        {
            if (amount > item.get_amount())
            {
                std::cout<<"The requested amount is not available in inventory"<<std::endl;
                break;
            }

            if (player.get_balance() < (item.get_price() * amount))
            {
                std::cout<<"The player's balance is not sufficient to make the purchase"<<std::endl;
                break;
            }

            if (item.get_category() == ItemType::Bus)
            {
                item.decrease_amount(amount);
                player.decrease_balance(item.get_price() * amount);

                for (auto bus : bus_list)
                {
                    if (bus.get_id() == item.get_id())
                    {
                        player.add_bus(bus);
                        break;
                    }
                }
            }
            else if (item.get_category() == ItemType::Employee)
            {
                item.decrease_amount(amount);
                player.decrease_balance(item.get_price() * amount);

                for (auto employee : employee_list)
                {
                    if (employee.get_id() == item.get_id())
                    {
                        player.add_employee(employee);
                        break;
                    }
                }
            }

            if (item.get_amount() == 0)
            {
                item.set_is_deleted(true);
            }

            break;
        }
    }
}

void Store::buy_bus_maintenance(int id, Player &player, bool repair_engine, bool repair_breaks, bool repair_tires, bool refuel)
{
    auto prices = player.get_bus(id).calc_maintenance_price();
    int total_price = 0;

    if (repair_engine)
    {
        total_price += prices.at(0);
    }
    if (repair_breaks)
    {
        total_price += prices.at(1);
    }
    if (repair_tires)
    {
        total_price += prices.at(2);
    }
    if (refuel)
    {
        total_price += prices.at(3);
    }

    if (player.get_balance() < total_price)
    {
        std::cout<<"The player's balance is not sufficient to make the purchase"<<std::endl;
    }
    else
    {
        player.get_bus(id).repair_bus(repair_engine, repair_breaks, repair_tires, refuel);
        player.decrease_balance(total_price);
    }
}

void Store::buy_bus_improvements(int id, Player &player, bool improve_engine, bool improve_breaks, bool improve_tires, bool improve_fuel)
{
    auto prices = player.get_bus(id).calc_improvements_price();
    int total_price = 0;

    if (improve_engine)
    {
        total_price += prices.at(0);
    }
    if (improve_breaks)
    {
        total_price += prices.at(1);
    }
    if (improve_tires)
    {
        total_price += prices.at(2);
    }
    if (improve_fuel)
    {
        total_price += prices.at(3);
    }

    if (player.get_balance() < total_price)
    {
        std::cout << "The player's balance is not sufficient to make the purchase" << std::endl;
    }
    else
    {
        player.get_bus(id).improve_bus(improve_engine, improve_breaks, improve_tires, improve_fuel);
        player.decrease_balance(total_price);
    }
}

void Store::pay_employee(int id, Player &player)
{
    int paycheck = player.get_employee(id).get_total_work_hours() * player.get_employee(id).get_hourly_rate();
    if (player.get_balance() < paycheck)
    {
        std::cout << "The player's balance is not sufficient to pay the employee" << std::endl;
    }
    else
    {
        player.decrease_balance(paycheck);
        player.get_employee(id).set_total_work_hours(0);
    }
}

void Store::claim_achievement(Player &player, Achievement &achievement)
{
    if (!achievement.is_claimed())
    {
        player.increase_balance(achievement.get_accumulated_rewards(true));
    }
    else
    {
        std::cout << "Achievement already claimed" << std::endl;
    }
}

void Store::pay_demand(Player &player, int amount)
{
    if (player.get_balance() < amount)
    {
        std::cout << "The player's balance is not sufficient to pay the demand" << std::endl;
    }
    else
    {
        player.decrease_balance(amount);
    }
}

void Store::pay_tow(Player &player, int id)
{
    auto tow = player.get_bus(id);
    buy_bus_maintenance(id, player, true, true, true, true);
    int amount = 5000;
    if (player.get_balance() < amount)
    {
        std::cout << "The player's balance is not sufficient to pay the tow fee" << std::endl;
    }
    else
    {
        player.decrease_balance(amount);
    }
}

nlohmann::json Store::to_json()
{
    nlohmann::json j;

    for (auto &item : inventory)
    {
        j["inventory"].push_back(item.to_json());
    }

    for (auto &bus : bus_list)
    {
        j["bus_list"].push_back(bus.to_json());
    }

    for (auto &employee : employee_list)
    {
        j["employee_list"].push_back(employee.to_json());
    }

    return j;
}

void Store::save()
{
    std::ofstream store_file("data/store.json");
    store_file << to_json().dump(4);
    store_file.close();
}