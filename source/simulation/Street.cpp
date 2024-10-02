#include <random>
#include <ctime>
#include "Street.hpp"

Street::Street() : id(0), name(""), distance(0), avg_travel_time(60), sd_travel_time(5), singular_event_odds(0.01)
{
    //empty
}


Street::Street(int _id, std::string _name, int _distance, float _avg_travel_time, float _sd_travel_time, float _singular_event_odds)
    : id(_id), name(_name), distance(_distance), avg_travel_time(_avg_travel_time), sd_travel_time(_sd_travel_time), singular_event_odds(_singular_event_odds)
{
    //empty
}

int Street::get_id() const noexcept
{
    return id;
}

std::string Street::get_name() const noexcept
{
    return name;
}

void Street::set_name(const std::string& _name)
{
    name = _name;
}

int Street::get_distance() const noexcept
{
    return distance;
}

int Street::get_travel_time() const
{

    // Seed with a real random value, if available
    std::random_device rd;

    // Initialize random number generator
    std::mt19937 gen(rd());

    std::normal_distribution<float> travel_time_dist(avg_travel_time, sd_travel_time);
    std::uniform_real_distribution<float> singular_event_dist{};

    int travel_time = static_cast<int>(travel_time_dist(gen));

    if(singular_event_dist(gen) < singular_event_odds)
    {
        std::uniform_int_distribution<int> singular_event_dist{300, 3600};
        travel_time += singular_event_dist(gen);
    }

    return travel_time;
}