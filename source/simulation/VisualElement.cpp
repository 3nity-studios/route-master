#include "simulation/VisualElement.hpp"

VisualElement::VisualElement() : id(0), x(0.f), y (0.f)
{
    //empty
}

VisualElement::VisualElement(int _id, float _x, float _y) : id(_id), x(_x), y(_y)
{
    //empty
}

VisualElement::VisualElement(nlohmann::json j)
{
    id = j["id"];
    x = j["x"];
    y = j["y"];
}

void VisualElement::set_id(int _id)
{
    id = _id;
}

void VisualElement::set_x(float _x) 
{
    x = _x;
}

void VisualElement::set_y(float _y)
{
    y = _y;
}

int VisualElement::get_id() const
{
    return id;
}

float VisualElement::get_x()
{
    return x;
}

float VisualElement::get_y()
{
    return y; 
}

nlohmann::json VisualElement::to_json()
{
    nlohmann::json j;

    j["id"] = id;
    j["x"] = x;
    j["y"] = y;

    return j;
}