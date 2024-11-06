#include "player/Player.hpp"
#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

TEST_CASE("Passenger to JSON", "[passenger_json]")
{
    Passenger passenger(10, 5, 3);
    nlohmann::json j = passenger.to_json();

    REQUIRE(j["arrival_time"] == 10);
    REQUIRE(j["waiting_time"] == 5);
    REQUIRE(j["bus_stop"] == 3);
}

TEST_CASE("Passenger from JSON", "[passenger_json]")
{
    nlohmann::json j;
    j["arrival_time"] = 15;
    j["waiting_time"] = 7;
    j["bus_stop"] = 2;

    Passenger passenger = Passenger(j);

    REQUIRE(passenger.get_arrival_time() == 15);
    REQUIRE(passenger.get_waiting_time() == 7);
    REQUIRE(passenger.get_bus_stop() == 2);
}

TEST_CASE("Bus to JSON", "[bus_json]")
{
    std::list<Passenger> passengers;
    Bus bus(0, "Bus1", 50, passengers, 10, 100, 500, 80, 300, 70, 200, 60, 100);
    nlohmann::json j = bus.to_json();
    
    REQUIRE(j["id"] == 0);
    REQUIRE(j["name"] == "Bus1");
    REQUIRE(j["max_capacity"] == 50);
    REQUIRE(j["time_in_bus_stop"] == 10);
    REQUIRE(j["features"]["engine"]["base_value"] == 100);
    REQUIRE(j["features"]["engine"]["base_price"] == 500);
    REQUIRE(j["features"]["breaks"]["base_value"] == 80);
    REQUIRE(j["features"]["breaks"]["base_price"] == 300);
    REQUIRE(j["features"]["tires"]["base_value"] == 70);
    REQUIRE(j["features"]["tires"]["base_price"] == 200);
    REQUIRE(j["features"]["fuel"]["base_value"] == 60);
    REQUIRE(j["features"]["fuel"]["base_price"] == 100);
}

TEST_CASE("Bus from JSON", "[bus_json]")
{
    nlohmann::json j;
    j["id"] = 0;
    j["name"] = "Bus2";
    j["max_capacity"] = 60;
    j["time_in_bus_stop"] = 15;
    j["features"]["engine"]["base_value"] = 110;
    j["features"]["engine"]["base_price"] = 550;
    j["features"]["breaks"]["base_value"] = 85;
    j["features"]["breaks"]["base_price"] = 320;
    j["features"]["tires"]["base_value"] = 75;
    j["features"]["tires"]["base_price"] = 220;
    j["features"]["fuel"]["base_value"] = 65;
    j["features"]["fuel"]["base_price"] = 120;

    Bus bus(j);

    REQUIRE(bus.get_id() == 0);
    REQUIRE(bus.get_name() == "Bus2");
    REQUIRE(bus.get_max_capacity() == 60);
    REQUIRE(bus.get_time_in_bus_stop() == 15);
    REQUIRE(bus.get_feature_info("engine").base_value == 110);
    REQUIRE(bus.get_feature_info("engine").base_price == 550);
    REQUIRE(bus.get_feature_info("breaks").base_value == 85);
    REQUIRE(bus.get_feature_info("breaks").base_price == 320);
    REQUIRE(bus.get_feature_info("tires").base_value == 75);
    REQUIRE(bus.get_feature_info("tires").base_price == 220);
    REQUIRE(bus.get_feature_info("fuel").base_value == 65);
    REQUIRE(bus.get_feature_info("fuel").base_price == 120);
}

TEST_CASE("Employee to JSON", "[employee_json]")
{
    Employee employee(0, "John", "Doe", 30, 20, 8, 5);
    nlohmann::json j = employee.to_json();

    REQUIRE(j["name"] == "John");
    REQUIRE(j["last_name"] == "Doe");
    REQUIRE(j["age"] == 30);
    REQUIRE(j["hourly_rate"] == 20);
    REQUIRE(j["shift_len"] == 8);
    REQUIRE(j["fatigue"] == 5);
}

TEST_CASE("Employee from JSON", "[employee_json]")
{
    nlohmann::json j;
    j["id"] = 0;
    j["name"] = "Jane";
    j["last_name"] = "Smith";
    j["age"] = 25;
    j["hourly_rate"] = 25;
    j["shift_len"] = 7;
    j["fatigue"] = 3;

    Employee employee = Employee(j);

    REQUIRE(employee.get_id() == 0);
    REQUIRE(employee.get_name() == "Jane");
    REQUIRE(employee.get_last_name() == "Smith");
    REQUIRE(employee.get_age() == 25);
    REQUIRE(employee.get_hourly_rate() == 25);
    REQUIRE(employee.get_shift_len() == 7);
    REQUIRE(employee.get_fatigue() == 3);
}

TEST_CASE("Player to JSON", "[player_json]")
{
    Player player(0, "Player1", 1000);
    Bus bus(0, "Bus1", 50, {}, 10, 100, 500, 80, 300, 70, 200, 60, 100);
    Employee employee(1, "John", "Doe", 30, 20, 8, 5);
    player.add_bus(bus);
    player.add_employee(employee);
    nlohmann::json j = player.to_json();

    REQUIRE(j["id"] == 0);
    REQUIRE(j["name"] == "Player1");
    REQUIRE(j["balance"] == 1000);
    REQUIRE(j["buses"].size() == 1);
    REQUIRE(j["employees"].size() == 1);
    REQUIRE(j["buses"][0]["id"] == 0);
    REQUIRE(j["employees"][0]["id"] == 0);
}

TEST_CASE("Player from JSON", "[player_json]")
{
    nlohmann::json j;
    j["id"] = 0;
    j["name"] = "Player1";
    j["balance"] = 2000;
    j["buses"] = {{{"id", 0},
                   {"name", "Bus2"},
                   {"max_capacity", 60},
                   {"time_in_bus_stop", 15},
                   {"features",
                    {{"engine", {"base_value", 110, "base_price", 550}},
                     {"breaks", {"base_value", 85, "base_price", 320}},
                     {"tires", {"base_value", 75, "base_price", 220}},
                     {"fuel", {"base_value", 65, "base_price", 120}}}}}};
    j["employees"] = {{{"id", 0},
                       {"name", "Jane"},
                       {"last_name", "Smith"},
                       {"age", 25},
                       {"hourly_rate", 25},
                       {"shift_len", 7},
                       {"fatigue", 3}}};

    Player player = Player(j);

    REQUIRE(player.get_id() == 0);
    REQUIRE(player.get_name() == "Player2");
    REQUIRE(player.get_balance() == 2000);
    REQUIRE(player.get_buses().size() == 1);
    REQUIRE(player.get_employees().size() == 1);
    REQUIRE(player.get_bus(2).get_id() == 0);
    REQUIRE(player.get_employee(2).get_id() == 0);
}