#pragma once

#include <string>
#include <vector>
#include <memory>
#include "player/Player.hpp"
#include "player/Store.hpp"

enum MessageType
{
    AchievementMessage,
    TowMessage,
    DemandMessage
};

class Message
{
    protected:
        std::string title;
        std::string content;
        MessageType type;
        int timestamp;

    public:
        Message();
        Message(const std::string &_title, const std::string &_content, MessageType _type, int _timestamp);

        std::string get_title() const;
        std::string get_content() const;
        int get_timestamp() const;
        MessageType get_type() const;

        void set_title(const std::string &_title);
        void set_content(const std::string &_content);
        void set_type(const MessageType &_type);
        void set_timestamp(int _timestamp);
        void execute(Store &store, Player &player, std::vector<Message> &messages);
};