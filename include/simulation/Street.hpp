#include <string>

class Street
{
    private:

    int id;
    std::string name;
    int distance;
    float avg_travel_time;
    float sd_travel_time;
    float singular_event_odds;

    public: 

    Street();
    Street(int _id, std::string _name, int _distance, float _avg_travel_time, float _sd_travel_time, float _singular_event_odds);

    int get_id();

    void set_name(std::string _name);
    std::string get_name();
    
    int get_distance();
    int get_travel_time();
};