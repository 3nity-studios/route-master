#include "player/Message.hpp"

Message::Message() : title(""), content(""), type(MessageType::AchievementMessage), timestamp(0)
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