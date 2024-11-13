#pragma once
#include "config/game.hpp"
#include "simulation/City.hpp"
#include "simulation/Street.hpp"
#include "simulation/VisualElement.hpp"
#include <cmath>
#include <ctime>
#include <random>

namespace util
{
static Street StreetFactory(GameDataRef game_data, int id, std::string name, int v_element1_id, int v_element2_id,
                            float avg_speed, float avg_traffic_density, float sd_traffic_density,
                            float singular_event_odds)
{
    static VisualElementNodeList visual_elements = game_data->city.get_visual_elements();
    std::shared_ptr<VisualElement> element1;
    std::shared_ptr<VisualElement> element2;

    // this loop sucks ik
    for (auto &element : visual_elements)
    {
        auto current_element = element->get_info();
        if (element->get_info()->get_id() == v_element1_id)
        {
            element1 = current_element;
        }
        else if (element->get_info()->get_id() == v_element2_id)
        {
            element2 = current_element;
        }
    }

    auto x = element1->get_x() - element2->get_x();
    auto y = element1->get_y() - element2->get_y();
    auto distance = std::hypot(x, y);

    Street street(id, name, distance, avg_speed, avg_traffic_density, sd_traffic_density, singular_event_odds);

    game_data->city.add_street(street, v_element1_id, v_element2_id);

    return street;
}
} // namespace util