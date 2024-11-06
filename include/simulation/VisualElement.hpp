#pragma once

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class VisualElement
{
    private:
    int id;
    float x;
    float y;

    public:
    VisualElement();
    VisualElement(int _id, float _x, float _y);
    VisualElement(nlohmann::json j);
    virtual ~VisualElement() = default;

    void set_id(int _id);
    void set_x(float _x);
    void set_y(float y);

    int get_id() const;
    float get_x();
    float get_y();

    nlohmann::json to_json();
};
