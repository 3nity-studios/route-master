#include <string>
#include <vector>

struct SingularEvent
{
    std::string name;
    float shape;
    int avg_duration; // in seconds
    float speed_reduction_factor; // percentage of usual speed
};

std::vector<SingularEvent> singular_event_parameters = 
{
    {"Road works", 2.0f, 300, 0.90f},
    {"Accident", 3.0f, 420, 0.80f},
    {"Inclement weather", 1.0f, 240, 0.70f},
};

class Street
{
    private:

    int id;
    std::string name;
    float distance;
    float avg_speed;
    float avg_traffic_density;
    float sd_traffic_density;
    float singular_event_odds;

    float current_traffic_density;
    bool singular_event_active;
    SingularEvent current_singular_event;
    int current_singular_event_duration;

    public: 

    Street();
    Street(int _id, std::string _name, float _distance, float _avg_speed, float _avg_traffic_density, float _sd_traffic_density, float _singular_event_odds);

    int get_id() const noexcept;
    std::string get_name() const noexcept;
    void set_name(const std::string& _name);
    
    int get_distance() const noexcept;
    int get_travel_time() const;

    void update();

    bool operator==(const Street& other) const {
        return this->id == other.id;
    }
};