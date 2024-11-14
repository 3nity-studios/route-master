#include "simulation/Route.hpp"

Route::Route() : name("") {}

Route::Route(std::string _name, std::vector<std::shared_ptr<VisualElement>> _route) : name(_name), route(_route) {}

Route::Route(nlohmann::json j, std::vector<std::shared_ptr<VisualElement>> visual_elements)
{
    name = j["name"]; 

    for (auto jvisual_element : j["visual_elements"])
    {
        VisualElement visual_element(jvisual_element);

        for (auto visual_element2 : visual_elements)
        {
            if (visual_element2->get_id() == visual_element.get_id())
            {
                route.push_back(visual_element2);
                break; 
            }
        }
    }
}

void Route::add_visual_element(std::shared_ptr<VisualElement> element)
{
    route.push_back(element);
}

nlohmann::json Route::to_json()
{
    nlohmann::json j; 

    nlohmann::json visual_elements = nlohmann::json::array();

    for (auto visual_element : this->route)
    {
        visual_elements.push_back(visual_element->to_json());
    }

    j["visual_elements"] = visual_elements; 
    j["name"] = name; 

    return j; 
}