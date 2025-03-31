/*
 * discord_bot.h
 *
 * handle connection with the Discord servers
 * uses the DPP library
 * https://dpp.dev/
 */

#ifndef DISCORD_BOT__H
#define DISCORD_BOT__H

#include <map>

#include "dpp/dpp.h"
#include "dpp/restresults.h"
#include "dpp/guild.h"

#include "lux_logger.h"

#include "discord_guild.h"
#include "discord_channel.h"

namespace luxbracer {
    /**
     * @class DiscordBot
     *
     * @brief The bot will run on the DPP library to connect to
     *        discord, and offer a callback function for message
     *        posting
     */
    class DiscordBot //: public luxbracer::CallbackClass
    {
        private:
            Logger * logger = NULL;
//            cdb::CallbackClass * m_handler = NULL;
            std::string _bot_id;
            std::map<dpp::snowflake, DiscordGuild *> guildMap;

        public:
            DiscordBot(void);

            /**
             * @brief Initiaze the bot
             *
             * @param token Thi discord authentication token
             */
            void init(std::string token, std::string bot_id);

            void add_guild(dpp::snowflake);

            void add_channel(std::string name, dpp::snowflake guild_id, dpp::snowflake channel, dpp::snowflake parent_id);

            /**
             * @brief set the logger object
             *
             * @param logger The logger object
             */
            void set_logger(Logger * logger);

            /**
             * @brief register commands
             ()
             */
            void slash_commands_register(dpp::snowflake guild_id);

            
            /**
             * @brief handle commands
             ()
             */
            void slash_commands_handle(const dpp::slashcommand_t & event);

            /**
             * @brief Post a message to discord
             *
             * @param msg The message information
             */
//            void message_cb(cdb::callback_msg *msg);

            /**
             * @brief Create a channel
             *
             * @param guild_id The server where teh category will be created
             * @param name The category name
             */
            void category_create(dpp::snowflake guild_id, dpp::snowflake parent_id, std::string name);

            /**
             * @brief Create a channel
             *
             * @param guild_id The server where teh channel will be created
             * @param name The channel name
             */
            void channel_create(dpp::snowflake guild_id, dpp::snowflake parent_id, std::string name, dpp::channel_type chanType);

            /**
             * @brief Delete a channel
             *
             * @param guild_id The server where teh channel is
             * @param name The channel name
             */
            void channel_delete(dpp::snowflake guild_id, std::string name);

            void slash_commands_handle_channel_create(const dpp::slashcommand_t & event);

            /**
             * @brief Getter function for the bot id
             *
             * @return The bot id
             */
            std::string bot_id(void);
    };
}

#endif /* DISCORD_BOT__H */
