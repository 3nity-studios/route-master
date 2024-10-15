#pragma once

#include <SFML/Graphics.hpp>

class VisualElement
{
    private:
    int id;
    float x;
    float y;

    public:
    VisualElement();
    VisualElement(int _id, float _x, float _y);
    virtual ~VisualElement() = default;

    void set_id(int _id);
    void set_x(float _x);
    void set_y(float y);

    int get_id() const;
    float get_x();
    float get_y();
};
