#include <iostream>
#include "City.hpp"
#include "Bus.hpp"

void test_passenger_generation()
{
    std::cout << "Testing passenger generation in BusStop ";
    
    BusStop bus_stop = BusStop(0, "A", 5, 5, 3, 3, 2);
    bus_stop.generate_passengers();
    std::cout << "OK!" << std::endl;
}

void test_get_passengers_on_bus()
{
    std::cout << "Testing get passengers on bus ";

    BusStop bus_stop = BusStop(0, "A", 5, 5, 3, 3, 2);
    bus_stop.generate_passengers();

    Bus bus = Bus("A", 15, std::list<Passenger>{}, 0);
    bus.add_passengers(3, bus_stop);

    bool test_passed = true;
    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_arrival_time() > 3)
        {
            test_passed = false;
            break;
        }
    }

    if (test_passed)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout << "FAIL!" << std::endl;
    }
}

void test_future_passengers_dont_get_on_bus()
{
    std::cout << "Testing that future passengers don't get on the bus ";

    BusStop bus_stop = BusStop(0, "A", 5, 5, 3, 3, 2);
    bus_stop.generate_passengers();

    Bus bus = Bus("A", 15, std::list<Passenger>{}, 2);
    bus.add_passengers(3, bus_stop); // Current timestep is 3

    bool test_passed = true;

    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_arrival_time() > 5)
        {
            test_passed = false;
            break;
        }
    }

    /*
    while(!bus_stop.get_passenger_list().empty())
    {
        Passenger passenger = bus_stop.pop_first_passenger();
        if (passenger.get_arrival_time() <= 5 || passenger.get_arrival_time() + passenger.get_waiting_time() >= 3)
        {
            test_passed = false;
            std::cout<<passenger.get_arrival_time()<<std::endl;
            std::cout<<passenger.get_waiting_time();

            break;

        }
    }
    */

    if (test_passed)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout << "FAIL!" << std::endl;
    }
}

void test_gone_passengers_dont_get_on_bus()
{
    std::cout << "Testing that gone passengers don't get on the bus ";

    BusStop bus_stop = BusStop(0, "A", 5, 5, 3, 3, 2);
    bus_stop.generate_passengers();

    Bus bus = Bus("A", 15, std::list<Passenger>{}, 2);
    bus.add_passengers(3, bus_stop); // Current timestep is 3

    bool test_passed = true;

    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_arrival_time() + passenger.get_waiting_time() < 3)
        {
            test_passed = false;
            break;
        }
    }

    /*
    while(!bus_stop.get_passenger_list().empty())
    {
        Passenger passenger = bus_stop.pop_first_passenger();
        if (passenger.get_arrival_time() + passenger.get_waiting_time() < 3)
        {
            test_passed = false;
            break;
        }
    }
    */

    if (test_passed)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout << "FAIL!" << std::endl;
    }
}

void test_leave_passengers()
{
    std::cout << "Testing leave passengers ";

    std::list<Passenger> passengers = {
        Passenger(0, 0, 1),
        Passenger(1, 0, 2),
        Passenger(2, 0, 3)
    };

    Bus bus = Bus("A", 15, passengers, 0);
    BusStop bus_stop(2, "Terminal", 5, 4, 3, 2, 1);

    bus.leave_passengers(bus_stop); // Current stop

    bool test_passed = true;

    for (auto passenger : bus.get_passenger_list())
    {
        if (passenger.get_bus_stop() == 2)
        {
            test_passed = false;
            break;
        }
    }

    if (test_passed)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout << "FAIL!" << std::endl;
    }
}

void test_city_constructor() {
    Designar::Graph<BusStop, Street> city_map;
    City city(1, "TestCity", city_map);
    std::cout << "Testing city constructor OK!" << std::endl;
}

void test_add_bus_stop() {
    std::cout << "Testing leave passengers ";

    City city;
    BusStop bus_stop(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    city.add_bus_stop(bus_stop);
    auto bus_stops = city.get_bus_stops();
    if (!bus_stops.is_empty() && bus_stops.get_first()->get_info() == bus_stop) {
        std::cout << "OK!" << std::endl;
    } else {
        std::cout << "FAIL!" << std::endl;
    }
}

void test_add_street() {
    std::cout << "Testing adding bus stop to city ";

    City city;
    BusStop stop1(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    BusStop stop2(2, "Stop2", 5.0, 5.0, 3.0, 3.0, 2.0);
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    Street street_info(1, "Street1", 100, 10.0f, 2.0f, 0.1f);
    city.add_street(street_info, 1, 2);
    auto streets = city.get_streets();
    if (!streets.is_empty() && streets.get_first()->get_info() == street_info) {
        std::cout << "OK!" << std::endl;
    } else {
        std::cout << "FAIL!" << std::endl;
    }
}

void test_run_simulation() {
    std::cout << "Testing run simulation ";

    City city;
    
    BusStop stop1(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    BusStop stop2(2, "Stop2", 10.0, 10.0, 3.0, 3.0, 2.0);
    BusStop stop3(3, "Stop3", 15.0, 15.0, 3.0, 3.0, 2.0);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops();
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++)
    {
        for (auto street : city.get_streets())
        {
            if(street->get_info().get_id() == i )
            {
                path.append(street);
            }
        }
    }
    
    Bus bus("Bus1", 15, std::list<Passenger>{}, 0);
    Employee driver("John", "Doe", 30, 8, 0);
    city.run_simulation(bus, driver, 10, path);
    std::cout << "OK!" << std::endl;
}

void test_driver_fatigue()
{
    std::cout << "Testing driver fatigue ";

    City city;
    
    BusStop stop1(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    BusStop stop2(2, "Stop2", 10.0, 10.0, 3.0, 3.0, 2.0);
    BusStop stop3(3, "Stop3", 15.0, 15.0, 3.0, 3.0, 2.0);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops();
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++)
    {
        for (auto street : city.get_streets())
        {
            if(street->get_info().get_id() == i )
            {
                path.append(street);
            }
        }
    }
    
    Bus bus("Bus1", 15, std::list<Passenger>{}, 0);
    Employee driver("John", "Doe", 30, 8, 0);
    city.run_simulation(bus, driver, 10, path);
    
    if (driver.get_fatigue() == 100)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout<< "FAIL!" << std::endl;
    }
}

void test_bus_wear()
{
    std::cout << "Testing bus wear ";

    City city;
    
    BusStop stop1(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    BusStop stop2(2, "Stop2", 10.0, 10.0, 3.0, 3.0, 2.0);
    BusStop stop3(3, "Stop3", 15.0, 15.0, 3.0, 3.0, 2.0);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops();
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++)
    {
        for (auto street : city.get_streets())
        {
            if(street->get_info().get_id() == i )
            {
                path.append(street);
            }
        }
    }
    
    Bus bus("Bus1", 15, std::list<Passenger>{}, 0);
    Employee driver("John", "Doe", 30, 8, 0);
    city.run_simulation(bus, driver, 10, path);
    
    if (bus.get_engine_state() == 90 && bus.get_breaks_state() == 80 && bus.get_tires_state() == 80 && bus.get_fuel() == 34)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout<< "FAIL!" << std::endl;
    }
}

void test_simulation_consistency()
{
    std::cout << "Testing simulation consistency ";

    City city;
    
    BusStop stop1(1, "Stop1", 5.0, 5.0, 3.0, 3.0, 2.0);
    BusStop stop2(2, "Stop2", 10.0, 10.0, 3.0, 3.0, 2.0);
    BusStop stop3(3, "Stop3", 15.0, 15.0, 3.0, 3.0, 2.0);
    
    city.add_bus_stop(stop1);
    city.add_bus_stop(stop2);
    city.add_bus_stop(stop3);

    city.initialize_bus_stops();

    int total_passengers = 0;

    for (auto stop : city.get_bus_stops())
    {
        total_passengers += stop->get_info().get_passenger_list().size();
    }
    
    Street street1(1, "Street1", 100, 10.0f, 2.0f, 0.1f);
    Street street2(2, "Street2", 100, 10.0f, 2.0f, 0.1f);
    
    city.add_street(street1, 1, 2);
    city.add_street(street2, 2, 3);
    
    StreetArcList path;
    
    for(int i = 1; i < 3; i++)
    {
        for (auto street : city.get_streets())
        {
            if(street->get_info().get_id() == i )
            {
                path.append(street);
            }
        }
    }
    
    Bus bus("Bus1", 15, std::list<Passenger>{}, 0);
    Employee driver("John", "Doe", 30, 8, 0);
    city.run_simulation(bus, driver, 10, path);

    int total_passengers_after_simulation = 0;

    for (auto bus_stop : city.get_bus_stops())
    {
        total_passengers_after_simulation += bus_stop->get_info().get_passenger_list().size();
    }

    total_passengers_after_simulation += bus.get_passenger_list().size();
    
    if (total_passengers_after_simulation == total_passengers)
    {
        std::cout << "OK!" << std::endl;
    }
    else
    {
        std::cout<< "FAIL!" << std::endl;
    }
}

int main()
{
    test_passenger_generation();
    test_get_passengers_on_bus();
    test_future_passengers_dont_get_on_bus();
    test_gone_passengers_dont_get_on_bus();
    test_leave_passengers();
    test_city_constructor();
    test_add_bus_stop();
    test_add_street();
    test_run_simulation();
    test_driver_fatigue();
    test_bus_wear();
    //test_simulation_consistency();

    return 0;
}