#include <catch2/catch_test_macros.hpp>
#include "player/Store.hpp"

TEST_CASE("Player constructor", "[player_constructor]") {
    Player player(0, "John", 0);
    REQUIRE(player.get_name() == "John");
    REQUIRE(player.get_id() == 0);
}

TEST_CASE("Player set_name", "[player_set_name]") {
    Player player(1, "Alice", 100);
    player.set_name("Bob");
    REQUIRE(player.get_name() == "Bob");
}

TEST_CASE("Player get_buses", "[player_get_buses]") {
    Player player(4, "Eve", 400);
    Bus bus1(0, "Bus1", 50, {}, 0);
    Bus bus2(1, "Bus2", 50, {}, 0);
    player.add_bus(bus1);
    player.add_bus(bus2);
    auto buses = player.get_buses();
    REQUIRE(buses.size() == 2);
    REQUIRE(buses[0].get_name() == "Bus1");
    REQUIRE(buses[1].get_name() == "Bus2");
}

TEST_CASE("Player get_employees", "[player_get_employees]") {
    Player player(5, "Frank", 500);
    Employee emp1("Emp1", "LastName1", 30, 8, 0);
    Employee emp2("Emp2", "LastName2", 25, 8, 0);
    player.add_employee(emp1);
    player.add_employee(emp2);
    auto employees = player.get_employees();
    REQUIRE(employees.size() == 2);
    REQUIRE(employees[0].get_name() == "Emp1");
    REQUIRE(employees[1].get_name() == "Emp2");
}

TEST_CASE("Player get_bus", "[player_get_bus]") {
    Player player(2, "Charlie", 200);
    Bus bus1(0, "Bus1", 50, {}, 0);
    Bus bus2(1, "Bus2", 50, {}, 0);
    Bus bus3(2, "Bus3", 50, {}, 0);
    player.add_bus(bus1);
    player.add_bus(bus2);
    player.add_bus(bus3);
    REQUIRE(player.get_bus(0).get_name() == "Bus1");
    REQUIRE(player.get_bus(1).get_name() == "Bus2");
    REQUIRE(player.get_bus(2).get_name() == "Bus3");
}

// TEST_CASE("Player get_employee", "[player_get_employee]") {
//     Player player(3, "Dave", 300);
//     Employee emp1("Emp1", "LastName1", 30, 8, 0);
//     Employee emp2("Emp2", "LastName2", 25, 8, 0);
//     player.add_employee(emp1);
//     player.add_employee(emp2);
//     REQUIRE(player.get_employee(0).get_name() == "Emp1");
//     REQUIRE(player.get_employee(1).get_name() == "Emp2");
// }