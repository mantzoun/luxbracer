#include "discord_bot.h"

dpp::command_completion_event_t  channels_callback(dpp::confirmation_callback_t value)
{
//    bot->log(dpp::ll_debug, "Callback");
   if ( value.is_error() == true ){
        dpp::error_info err = value.get_error();
    //    bot->log(dpp::ll_error, "Error " + err.message);
    printf("Error %s", err.message);
   }
//
//    dpp::guild_map guildmap = std::get<dpp::guild_map>(value.value);
//
//    dpp::snowflake id;
//    dpp::guild g;
//
//    for (auto& it: guildmap) {
//        // Do stuff
//        id = it.first;
//        g = it.second;
//
//        bot->channels_get(id, &channels_cb);
//    }
    return NULL;
}

void luxbracer::DiscordBot::channel_create(dpp::cluster * bot, std::string name) {
    dpp::channel * chan = new dpp::channel();

    chan->name = "test";

    bot->channel_create(*chan, channels_callback); 	
}

