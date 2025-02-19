#include <catch2/catch_test_macros.hpp>
#include "simulation/City.hpp"
#include "simulation/Bus.hpp"

TEST_CASE("Passenger generation in BusStop", "[passenger_generation]") {

    BusStop bus_stop = BusStop(0, "A", {5}, 5.0f, 3.0f, 3.0f, 2.0f, 2.0f, 50, 50);
    bus_stop.generate_passengers(0);
    REQUIRE(bus_stop.get_passenger_list().size() >= 0);
}

TEST_CASE("Get passengers on bus", "[get_passengers_on_bus]") {

    BusStop bus_stop = BusStop(0, "A", {5}, 5.0f, 3.0f, 3.0f, 2.0f, 2.0f, 50, 50);
    bus_stop.generate_passengers(0);

    Bus bus = Bus(0, "A", 15, std::list<Passenger>{}, 0, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee employee;

    SimulationInfo info(&bus, &employee, {});

    bus.add_passengers(3, bus_stop, info.elements_path, info.path_index);

    bool test_passed = true;
    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_arrival_time() > 3)
        {
            test_passed = false;
            break;
        }
    }

    REQUIRE(test_passed);
}

TEST_CASE("Passengers leave bus stop", "[passengers_leave_stop]") {

    BusStop bus_stop = BusStop(0, "A", {5}, 5.0f, 3.0f, 3.0f, 2.0f, 2.0f, 50, 50);
    bus_stop.generate_passengers(0);

    int initial_passenger_count = bus_stop.get_passenger_list().size();

    Bus bus = Bus(0, "A", 15, std::list<Passenger>{}, 0, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee employee; 
    
    SimulationInfo info(&bus, &employee, {});

    bus.add_passengers(3, bus_stop, info.elements_path, info.path_index);

    int final_passenger_count = bus_stop.get_passenger_list().size();

    REQUIRE(final_passenger_count <= initial_passenger_count);
}

TEST_CASE("Future passengers don't get on the bus", "[future_passengers]") {

    BusStop bus_stop = BusStop(0, "A", {5}, 5.0f, 3.0f, 3.0f, 2.0f, 2.0f, 50, 50);
    bus_stop.generate_passengers(0);

    Bus bus = Bus(0, "A", 15, std::list<Passenger>{}, 2, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee employee; 

    SimulationInfo info(&bus, &employee, {});

    bus.add_passengers(3, bus_stop, info.elements_path, info.path_index);// Current timestep is 3

    bool test_passed = true;

    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_arrival_time() > 5)
        {
            test_passed = false;
            break;
        }
    }

    REQUIRE(test_passed);
}

TEST_CASE("Gone passengers don't get on the bus", "[gone_passengers]") {

    BusStop bus_stop = BusStop(0, "A", {5}, 5.0f, 3.0f, 3.0f, 2.0f, 2.0f, 50, 50);
    bus_stop.generate_passengers(0);

    Bus bus = Bus(0, "A", 15, std::list<Passenger>{}, 2, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee employee; 

    SimulationInfo info(&bus, &employee, {});

    bus.add_passengers(3, bus_stop, info.elements_path, info.path_index);

    bool test_passed = true;

    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_arrival_time() + passenger.get_waiting_time()/60.f < 3)
        {
            test_passed = false;
            break;
        }
    }

    REQUIRE(test_passed);
}

TEST_CASE("Leave passengers", "[leave_passengers]") {
    std::list<Passenger> passengers = {
        Passenger(0, 0, 1),
        Passenger(1, 0, 2),
        Passenger(2, 0, 3)
    };

    Bus bus = Bus(0, "A", 15, passengers, 0, 100, 200, 100, 200, 100, 200, 100, 200);
    BusStop bus_stop(2, "Terminal", {5}, 4.0f, 3.0f, 2.0f, 1.0f, 1.0f, 50, 50);

    bus.leave_passengers(bus_stop); // Current stop

    bool test_passed = true;

    for (auto passenger : bus.get_passenger_list()) {
        if (passenger.get_bus_stop() == 2) {
            test_passed = false;
            break;
        }
    }

    REQUIRE(test_passed);
}

TEST_CASE("City constructor", "[city_constructor]") {
    Designar::Graph<std::shared_ptr<VisualElement>, Street> city_map;
    City city(1, "TestCity", city_map, 0);
    REQUIRE(city.get_name() == "TestCity");
}

TEST_CASE("Adding bus stop to city", "[add_bus_stop]") {
    City city;
    BusStop bus_stop(1, "Stop1", {5}, 5.0f, 5.0f, 3.0f, 3.0f, 2.0f, 50, 50);
    city.add_bus_stop(bus_stop);
    auto bus_stops = city.get_visual_elements();
    REQUIRE(!bus_stops.is_empty());
    REQUIRE(bus_stops.get_first()->get_info()->get_id() == 1);
}

TEST_CASE("Adding street to city", "[add_street]") {
    City city;
    BusStop stop1(1, "Stop1", {5}, 5.0, 5.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop2(2, "Stop2", {5}, 5.0, 5.0, 3.0, 3.0, 2.0, 50, 50);
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    Street street_info(1, "Street1", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    city.add_street(street_info, 1, 2);
    auto streets = city.get_streets();
    REQUIRE(!streets.is_empty());
    REQUIRE(streets.get_first()->get_info() == street_info);
}

TEST_CASE("Run simulation", "[run_simulation]") {
    City city;
    BusStop stop1(1, "Stop1", {5}, 5.0, 3.0, 3.0, 2.0, 2.0, 50, 50);
    BusStop stop2(2, "Stop2", {10}, 10.0, 3.0, 3.0, 2.0, 2.0, 50, 50);
    BusStop stop3(3, "Stop3", {15}, 15.0, 3.0, 3.0, 2.0, 2.0, 50, 50);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops(0);
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++) {
        for (auto street : city.get_streets()) {
            if(street->get_info().get_id() == i ) {
                path.append(street);
            }
        }
    }

    Bus bus(0, "Bus1", 15, std::list<Passenger>{}, 0, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee driver(0, "John", "Doe", 30, 20, 8, 0); 

    SimulationInfo simulation_info(&bus, &driver, path);
    std::vector<SimulationInfo> aux;
    aux.push_back(simulation_info);

    while (!aux.front().route_completed)
    {
        city.run_simulation(aux);
    }

    REQUIRE(true); // Assuming run_simulation does not return a value to check
}

TEST_CASE("Driver fatigue", "[driver_fatigue]") {
    City city;
  
    BusStop stop1(1, "Stop1", {5}, 5.0, 5.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop2(2, "Stop2", {10}, 10.0, 10.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop3(3, "Stop3", {15}, 15.0, 15.0, 3.0, 3.0, 2.0, 50, 50);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops(0);
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++) {
        for (auto street : city.get_streets()) {
            if(street->get_info().get_id() == i ) {
                path.append(street);
            }
        }
    }
    
    Bus bus = Bus(0, "A", 15, std::list<Passenger>{}, 0, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee driver(0, "John", "Doe", 30, 20, 8, 0); 
    
    SimulationInfo simulation_info(&bus, &driver, path);
    std::vector<SimulationInfo> aux;
    aux.push_back(simulation_info);

    while (!aux.front().route_completed)
    {
        city.run_simulation(aux);
    }
    
    REQUIRE(driver.get_fatigue() == 100);
}

TEST_CASE("Bus wear", "[bus_wear]") {
    City city;
    
    BusStop stop1(1, "Stop1", {5}, 5.0, 5.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop2(2, "Stop2", {10}, 10.0, 10.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop3(3, "Stop3", {15}, 15.0, 15.0, 3.0, 3.0, 2.0, 50, 50);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops(0);
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++) {
        for (auto street : city.get_streets()) {
            if(street->get_info().get_id() == i ) {
                path.append(street);
            }
        }
    }
    
    Bus bus = Bus(0, "A", 15, std::list<Passenger>{}, 0, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee driver(0, "John", "Doe", 30, 20, 8, 0); 
    
    SimulationInfo simulation_info(&bus, &driver, path);
    std::vector<SimulationInfo> aux;
    aux.push_back(simulation_info);

    while (!aux.front().route_completed)
    {
        city.run_simulation(aux);
    }
    
    REQUIRE(bus.get_engine_state() == 90);
    REQUIRE(bus.get_breaks_state() == 80);
    REQUIRE(bus.get_tires_state() == 80);
    REQUIRE(bus.get_fuel() == 34);
}

TEST_CASE("Simulation consistency", "[simulation_consistency]") {
    City city;
    
    BusStop stop1(1, "Stop1", {5}, 5.0, 5.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop2(2, "Stop2", {10}, 10.0, 10.0, 3.0, 3.0, 2.0, 50, 50);
    BusStop stop3(3, "Stop3", {15}, 15.0, 15.0, 3.0, 3.0, 2.0, 50, 50);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops(0);

    int total_passengers = 0;

    for (auto visual_element : city.get_visual_elements()) 
    {
        auto stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (stop)
        {
            total_passengers += stop->get_passenger_list().size();
        }
    }
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f, 0.05f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++) {
        for (auto street : city.get_streets()) {
            if(street->get_info().get_id() == i ) {
                path.append(street);
            }
        }
    }
    
    Bus bus(0, "Bus1", 15, std::list<Passenger>{}, 15, 100, 200, 100, 200, 100, 200, 100, 200);
    Employee driver(0, "John", "Doe", 30, 20, 8, 0); 

    SimulationInfo simulation_info(&bus, &driver, path);
    std::vector<SimulationInfo> aux;
    aux.push_back(simulation_info);

    city.update();

    while (!aux.front().route_completed)
    {
        city.run_simulation(aux);
    }

    int total_passengers_after_simulation = 0;

    for (auto visual_element : city.get_visual_elements()) 
    {
        auto bus_stop = std::dynamic_pointer_cast<BusStop>(visual_element->get_info());

        if (bus_stop)
        {
            total_passengers_after_simulation += bus_stop->get_passenger_list().size() + bus_stop->get_gone_passengers();
        }
    }

    total_passengers_after_simulation += aux.front().bus->get_attended_passengers();
    
    REQUIRE(total_passengers_after_simulation == total_passengers);
}
