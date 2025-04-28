#ifndef DISCORD_GUILD__H
#define DISCORD_GUILD__H

#include "dpp/dpp.h"
#include "discord_channel.h"
#include "lux_logger.h"
namespace luxbracer {
    class DiscordGuild {
        private:
            dpp::snowflake  _id;
            std::list<DiscordChannel> channels;
            Logger * logger;

        public:
            DiscordGuild(const dpp::snowflake _id);

            dpp::snowflake id(void) const;

            void set_logger(Logger * logger);

            void verify_and_add_channel(const DiscordChannel& channel);
            void verify_and_delete_channel(const DiscordChannel& channel);
            void verify_and_update_channel(const DiscordChannel& channel);

            void channel_add(const dpp::channel_create_t & channel);
            void channel_add(const dpp::channel & channel);
            void channel_delete(const dpp::channel_delete_t & channel);
            void channel_update(const dpp::channel_update_t & channel);

            std::list<DiscordChannel *> channel_get(std::string name, dpp::snowflake channel_id = 0, dpp::snowflake parent_id = 0);
    };
}

#endif /* DISCORD_GUILD__H */