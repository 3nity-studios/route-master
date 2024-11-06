#include "player/Store.hpp"
#include <iostream>

Store::Store() : inventory(Designar::SLList<Item>{}), bus_list(Designar::SLList<Bus>{}), employee_list(Designar::SLList<Employee>{})
{
}

Store::Store(Designar::SLList<Item> _inventory, Designar::SLList<Bus> _bus_list,
             Designar::SLList<Employee> _employee_list) : inventory(_inventory), bus_list(_bus_list), employee_list(_employee_list)
{
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