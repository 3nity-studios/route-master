#pragma once

#include "config/game.hpp"
#include "config/global.hpp"
#include "states/simulation_state.hpp"
#include "states/state.hpp"
#include "player/Message.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class MessagesState : public Engine::State
{
  public:
    MessagesState(GameDataRef data);
    //~MessagesState() = default;

    void init_state();
    void update_inputs();
    void update_state(float dt __attribute__((unused)));
    void draw_state(float dt __attribute__((unused)));

  private:
    GameDataRef _data;
    tgui::ScrollablePanel::Ptr messages_panel;
    Designar::SLList<Item> items_to_show;
    tgui::Panel::Ptr create_message_box(Message &message, int message_number);
};
