#include "player/Message.hpp"
#include "player/Achievement.hpp"

class AchievementMessage : public Message
{
    private:

    public:
        AchievementMessage(Achievement _achievement);
        void execute(Achievement achievement, Player player);
};