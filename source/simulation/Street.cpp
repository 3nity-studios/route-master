#include "simulation/Street.hpp"
#include <ctime>
#include <random>

Street::Street()
    : id(0), name(""), distance(0), avg_speed(0), avg_traffic_density(1), sd_traffic_density(),
      singular_event_odds(0.01)
{
    // empty
}

Street::Street(int _id, std::string _name, float _distance, float _avg_speed, float _avg_traffic_density, float _sd_traffic_density, float _singular_event_odds)
    : id(_id), name(std::move(_name)), distance(_distance), avg_speed(_avg_speed), avg_traffic_density(_avg_traffic_density), sd_traffic_density(_sd_traffic_density), singular_event_odds(_singular_event_odds),
      current_traffic_density(0.0f), singular_event_active(false), current_singular_event_duration(0)
{
    // empty
}

int Street::get_id() const noexcept
{
    return id;
}

std::string Street::get_name() const noexcept
{
    return name;
}

void Street::set_name(const std::string &_name)
{
    name = _name;
}

int Street::get_distance() const noexcept
{
    return distance;
}

int Street::get_travel_time() const
{
    float speed = avg_speed;
    speed -= current_traffic_density * 0.1;
    if (singular_event_active)
    {
        speed *= current_singular_event.speed_reduction_factor;
    }

    return std::round(distance / speed);
}

void Street::update()
{
    // Seed with a real random value, if available
    std::random_device rd;

    // Initialize random number generator
    std::mt19937 gen(rd());

    std::normal_distribution<float> traffic_density_dist(avg_traffic_density, sd_traffic_density);
    std::bernoulli_distribution singular_event_dist(singular_event_odds);

    current_traffic_density = traffic_density_dist(gen);

    if (singular_event_active)
    {
        --current_singular_event_duration;
        if (current_singular_event_duration == 0)
        {
            singular_event_active = false;
        }
    }
    else
    {
        if (singular_event_dist(gen))
        {
            std::uniform_int_distribution<> event_selector(0, singular_event_parameters.size() - 1);
            SingularEvent current_singular_event = singular_event_parameters[event_selector(gen)];
            std::weibull_distribution<> singular_event_time(current_singular_event.shape, current_singular_event.avg_duration);

            singular_event_active = true;
            current_singular_event_duration = singular_event_time(gen);
        }
    }
}