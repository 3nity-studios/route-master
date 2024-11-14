#pragma once
#include "simulation/City.hpp"

struct Route
{
    std::string name;
    std::vector<std::shared_ptr<VisualElement>> route;

    Route();
    Route(std::string _name, std::vector<std::shared_ptr<VisualElement>> _route);
    Route(nlohmann::json j, std::vector<std::shared_ptr<VisualElement>> visual_elements); 
    
    void add_visual_element(std::shared_ptr<VisualElement> element);
    nlohmann::json to_json(); 
};