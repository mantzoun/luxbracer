#ifndef DISCORD_GUILD__H
#define DISCORD_GUILD__H

#include "dpp/dpp.h"
#include "discord_channel.h"

namespace luxbracer {
    class DiscordGuild {
        private:
            uint64_t  _id;
            std::list<DiscordChannel *> channels;

        public:
            DiscordGuild(uint64_t);

            uint64_t  id(void);
            void channel_add(DiscordChannel *);
            std::list<DiscordChannel *> channel_get(std::string name, uint64_t  channel_id = 0, uint64_t  parent_id = 0);
            void channel_delete(uint64_t  channel_id, uint64_t  parent_id, std::string name);
    };
}

#endif /* DISCORD_GUILD__H */