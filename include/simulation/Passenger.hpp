#include <string>
#include <nlohmann/json.hpp>

class Passenger
{
    private:

    float arrival_time;
    float waiting_time;
    int bus_stop;

    public:

    Passenger();
    Passenger(float _arrival_time, float _waiting_time, int _bus_stop);
    Passenger(nlohmann::json j);

    float get_arrival_time() const noexcept;
    float get_waiting_time() const noexcept;
    int get_bus_stop() const noexcept;
    bool is_gone(float current_time) const noexcept;

    void set_arrival_time(const float& _arrival_time);
    void set_waiting_time(const float& _waiting_time);
    void set_bus_stop(const int& _bus_stop);

    nlohmann::json to_json();
};
