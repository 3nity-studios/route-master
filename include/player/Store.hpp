#include <Designar/list.hpp>
#include "player/Item.hpp"
#include "player/Player.hpp"

class Store
{
    private:
    Designar::SLList<Item> inventory; 
    Designar::SLList<Bus> bus_list;
    Designar::SLList<Employee> employee_list; 

    public: 

    Store();
    Store(Designar::SLList<Item> _inventory, Designar::SLList<Bus> _bus_list, Designar::SLList<Employee> _employee_list);

    Designar::SLList<Item> get_inventory();
    Designar::SLList<Bus> get_bus_list();
    Designar::SLList<Employee> get_employee_list();

    void set_inventory(Designar::SLList<Item> _inventory);
    void set_bus_list(Designar::SLList<Bus> _bus_list);
    void set_employee_list(Designar::SLList<Employee> _employee_list);

    void add_bus_to_inventory(Bus bus, int price, int amount);
    void add_employee_to_inventory(Employee employee, int price, int amount);

    void buy_item(Player &player, int item_id, int amount);
    void buy_bus_maintenance(int id, Player &player, bool repair_engine, bool repair_breaks, bool repair_tires);
};