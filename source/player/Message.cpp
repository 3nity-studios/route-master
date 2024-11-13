#include "player/Message.hpp"

Message::Message() : title(""), content(""), type(MessageType::AchievementMessage), timestamp(0)
{
    // empty
}

Message::Message(const std::string &_title, const std::string &_content, MessageType _type, int _timestamp)
    : title(_title), content(_content), type(_type), timestamp(_timestamp)
{
    // empty
}

std::string Message::get_title() const
{
    return title;
}

void Message::set_title(const std::string &_title)
{
    title = _title;
}

std::string Message::get_content() const
{
    return content;
}

void Message::set_content(const std::string &_content)
{
    content = _content;
}

int Message::get_timestamp() const
{
    return timestamp;
}

void Message::set_timestamp(int _timestamp)
{
    timestamp = _timestamp;
}

MessageType Message::get_type() const
{
    return type;
}

void Message::set_type(const MessageType &_type)
{
    type = _type;
}

void Message::execute(Store &store, Player &player, std::vector<std::shared_ptr<Message>> &messages)
{
    switch (type)
    {
    case MessageType::AchievementMessage:
        // store.claim_achievement(player, achievement);
        break;
    case MessageType::TowMessage:
        // store.pay_tow(player, bus_id);
        break;
    case MessageType::DemandMessage:
        // store.pay_demand(player, amount);
        break;
    default:
        break;
    }

    // Remove the message from the vector
    for (auto it = messages.begin(); it != messages.end(); ++it)
    {
        if ((*it)->get_timestamp() == timestamp)
        {
            messages.erase(it);
            break;
        }
    }
}