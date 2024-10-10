#include "simulation/VisualElement.hpp"

VisualElement::VisualElement() : x(0.f), y (0.f)
{
}

VisualElement::VisualElement(float _x, float _y) : x(_x), y(_y)
{
}

void VisualElement::set_x(float _x)
{
    x = _x;
}

void VisualElement::set_y(float _y)
{
    y = _y;
}

float VisualElement::get_x()
{
    return x;
}

float VisualElement::get_y()
{
    return y; 
}