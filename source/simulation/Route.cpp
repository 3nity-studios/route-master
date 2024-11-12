#include "simulation/Route.hpp"

Route::Route() : name("") {}

Route::Route(std::string _name, std::vector<std::shared_ptr<VisualElement>> _route) : name(_name), route(_route) {}

void Route::add_visual_element(std::shared_ptr<VisualElement> element)
{
    route.push_back(element);
}