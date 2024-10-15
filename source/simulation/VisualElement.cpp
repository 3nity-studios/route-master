#include "simulation/VisualElement.hpp"

VisualElement::VisualElement() : id(0), x(0.f), y (0.f)
{
}

VisualElement::VisualElement(int _id, float _x, float _y) : id(_id), x(_x), y(_y)
{
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