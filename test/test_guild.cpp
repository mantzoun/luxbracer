#include "ext/catch.hpp"
#include "include/discord_guild.h"

namespace luxbracer {
    struct GuildTestcase {
        GuildTestcase()
            : logger(LUX_LOG_DEBUG)
        {
        }

        ~GuildTestcase() {
        }

        Logger logger;
    };


    TEST_CASE_METHOD(GuildTestcase, "CheckDeleteMissingChannel", "GUILD_GROUP") {
        DiscordGuild guild(0);
        guild.set_logger(&logger);

        dpp::channel_delete_t channel;
        channel.deleted.name = "missing channel";
        channel.deleted.id = 0;
        channel.deleted.parent_id = 1;

        REQUIRE(guild.channel_delete(channel) == -1);
    }
}
