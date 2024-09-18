
#pragma once

#include <memory>
#include <stack>

#include "config/global.hpp"
#include "states/state.hpp"

namespace Engine
{

typedef std::unique_ptr<State> StateRef;

class StateMachine
{

  private:
    std::stack<Engine::StateRef> _states;
    Engine::StateRef _new_state;

    bool _is_removing;
    bool _is_adding;
    bool _is_replacing;

  public:
    StateMachine()
    {
    }
    ~StateMachine()
    {
    }

    void add_state(StateRef new_state, bool is_replacing);
    void remove_state();
    void process_states();

    StateRef &get_active_state();
};
} // namespace Engine
