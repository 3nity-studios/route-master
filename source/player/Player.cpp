#include "player/Player.hpp"

Player::Player() : id(0), name(""), balance(0)
{
    // empty
}

Player::Player(int _id, const std::string& _name, int _balance)
    : id(_id), name(_name), balance(_balance)
{
    // empty
}

int Player::get_id() const noexcept {
    return id;
}

std::string Player::get_name() const noexcept {
    return name;
}

void Player::set_name(const std::string& _name) {
    name = _name;
}

Designar::SLList<Bus> Player::get_buses()
{
    return buses.to_list();
}

Designar::SLList<Employee> Player::get_employees()
{
    return employees.to_list();
}

Bus &Player::get_bus(const int& id) {
    return buses.select(id);
}

Employee &Player::get_employee(const int& id) {
    return employees.select(id);
}

int Player::get_balance()
{
    return balance;
}

void Player::add_bus(Bus bus)
{
    bus.set_id(buses.size());
    buses.insert(bus);
}

void Player::add_employee(Employee employee)
{
    employee.set_id(employees.size());
    employees.insert(employee);
}

void Player::increase_balance(int balance_to_increase)
{
    balance += balance_to_increase;
}

void Player::decrease_balance(int balance_to_decrease)
{
    balance -= balance_to_decrease;
}
