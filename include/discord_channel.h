#ifndef DISCORD_CHANNEL__H
#define DISCORD_CHANNEL__H

#include "dpp/dpp.h"


namespace luxbracer {
    class DiscordChannel {
        private:
            uint64_t  _id;
            uint64_t  _parent;
            std::string _name;
        public:
            DiscordChannel(uint64_t  id, uint64_t  parent, std::string name);

            uint64_t  id(void);
            uint64_t  parent(void);
            std::string name(void);

    };
}

#endif /* DISCORD_CHANNEL__H */