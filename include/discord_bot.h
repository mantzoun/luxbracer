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
            Logger *        logger = NULL;
            std::string     bot_id;
            DiscordGuild *  guild;

            dpp::cluster    * discord_iface;
            dpp::snowflake  guild_id;
            std::map<std::string, dpp::snowflake> channels_list;

        public:
            DiscordBot(void);

            /**
             * @brief Initiaze the bot
             *
             * @param token Thi discord authentication token
             */
            void init(std::string token, std::string id);

            void initialize_guild(dpp::snowflake);

            void channel_added_callback(std::string name, dpp::snowflake channel, dpp::snowflake parent_id);
            void channel_deleted_callback(std::string name, dpp::snowflake channel, dpp::snowflake parent_id);

            /**
             * @brief set the logger object
             *
             * @param logger The logger object
             */
            void set_logger(Logger * logger);

            /**
             * @brief Initialize commands
             ()
             */
            void slash_commands_init();


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
            void channel_create(dpp::snowflake parent_id, std::string name, dpp::channel_type chanType);

            /**
             * @brief Delete a channel
             *
             * @param name The channel name
             */
            void channel_delete(std::string name);

            void slash_commands_handle_channel_create(const dpp::slashcommand_t & event);
            void slash_commands_handle_channel_delete(const dpp::slashcommand_t & event);
            void register_guild_commands(void);

            /**
             * @brief Getter function for the bot id
             *
             * @return The bot id
             */
            std::string bot_id_get(void);

            /**
             * @brief Post a message to a channel
             *
             * @param channel The channel to post in
             * @param message The text to post
             *
             * @return The bot id
             */
            void post_message(std::string channel, std::string message);

            dpp::command_completion_event_t user_get_guilds_callback(dpp::confirmation_callback_t value);
            dpp::command_completion_event_t channels_get_callback(dpp::confirmation_callback_t value);
            dpp::command_completion_event_t delete_global_commands(dpp::confirmation_callback_t value);
            dpp::command_completion_event_t delete_guild_commands(dpp::confirmation_callback_t value);
            dpp::command_completion_event_t guild_command_delete_counter(dpp::confirmation_callback_t value);
            dpp::command_completion_event_t channels_create_cb(dpp::confirmation_callback_t value);
    };
}

#endif /* DISCORD_BOT__H */
