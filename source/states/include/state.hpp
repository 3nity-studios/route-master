#pragma once

#include "SFML/Window/Event.hpp"
namespace Engine
{
class State
{
  public:
    // SHOULD BE VIRTUAL
    State()
    {
    }
    virtual ~State()
    {
    }

    virtual void init_state() = 0;
    virtual void update_inputs() = 0;
    virtual void update_state(float dt) = 0;
    virtual void draw_state(float dt) = 0;
    virtual void pause_state()
    {
    }
    virtual void resume_state()
    {
    }

    sf::Event event;
};
} // namespace Engine
