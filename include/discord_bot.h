/*
 * discord_bot.h
 *
 * handle connection with the Discord servers
 * uses the DPP library
 * https://dpp.dev/
 */

#ifndef DISCORD_BOT__H
#define DISCORD_BOT__H

#include "dpp/dpp.h"
#include "dpp/restresults.h"
#include "dpp/guild.h"

//#include "cdb_logger.h"
//#include "cdb_mqtt_handler.h"

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
//            cdb::Logger * logger = NULL;
//            cdb::CallbackClass * m_handler = NULL;
            std::string _bot_id;

        public:
            DiscordBot(void);

            /**
             * @brief Initiaze the bot
             *
             * @param token Thi discord authentication token
             */
            void init(std::string token, std::string bot_id);

            /**
             * @brief set the logger object
             *
             * @param logger The logger object
             */
//            void set_logger(cdb::Logger * logger);

            /**
             * @brief set the MQTT handler
             *
             * @param m_handler the MQTT handler object
             */
//            void set_mqtt_handler(cdb::CallbackClass * m_handler);

            /**
             * @brief Post a message to discord
             *
             * @param msg The message information
             */
//            void message_cb(cdb::callback_msg *msg);

            /**
             * @brief Create a channel
             *
             * @param name The channel name
             */
            void channel_create(dpp::cluster *, std::string name);

            /**
             * @brief Getter function for the bot id
             *
             * @return The bot id
             */
            std::string bot_id(void);
    };
}

#endif /* DISCORD_BOT__H */
