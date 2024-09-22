#include <iostream>
#include <Bus.hpp>

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
    bus.leave_passengers(2); // Current stop

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

int main()
{
    test_passenger_generation();
    test_get_passengers_on_bus();
    test_future_passengers_dont_get_on_bus();
    test_gone_passengers_dont_get_on_bus();
    test_leave_passengers();

    return 0;
}