#include <SFML/Graphics.hpp>

class VisualElement
{
    private:
    float x;
    float y;

    public:
    VisualElement();
    VisualElement(float _x, float _y);

    void set_x(float _x);
    void set_y(float y);

    float get_x();
    float get_y();
};
