#include "discord_bot.h"

extern dpp::cluster     * bot;

namespace luxbracer {
    void DiscordBot::slash_commands_register(dpp::snowflake guild_id) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand newcommand("channel_create", "Create a new channel", bot->me.id);
                    newcommand.add_option(
                        dpp::command_option(dpp::co_string, "name", "the channel name", true)
                    );
                    newcommand.add_option(
                        dpp::command_option(dpp::co_string, "parent", "the channel parent", false)
                    );
                    newcommand.add_option(
                        dpp::command_option(dpp::co_string, "type", "the channel type", false)
                            .add_choice(dpp::command_option_choice("Text", std::string("TextChannel")))
                            .add_choice(dpp::command_option_choice("Category", std::string("Category")))
                    );
            bot->log(dpp::ll_warning, "REGISTER");
            bot->guild_command_create(newcommand, guild_id);
            bot->global_command_create(newcommand);
        }
    }
    void DiscordBot::slash_commands_handle(const dpp::slashcommand_t & event) {
	        /* Check which command they ran */
            bot->log(dpp::ll_warning, "COMMAND");
	        if (event.command.get_command_name() == "channel_create") {
	            /* Fetch a parameter value from the command parameters */
	            //std::string animal = std::get<std::string>(event.get_parameter("animal"));
	 
	            /* Reply to the command. There is an overloaded version of this
	            * call that accepts a dpp::message so you can send embeds.
	            */
	            event.reply(std::string("Blep! You chose"));
	        }
	}
}