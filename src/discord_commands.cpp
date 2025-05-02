#include "discord_bot.h"


static std::vector<std::string> delete_commands = {};
static std::vector<std::string> existing_commands = {};

static std::vector<std::vector<std::string>> guild_commands = {
    {"channel_create", "Create a new channel",
        "string", "name", "the channel name", "true",
        "string", "parent", "the channel parent", "false",
        "string_choice", "type", "the channel type", "false",
            "choice", "Text", "TextChannel",
            "choice", "Category", "Category",
    },
    {"channel_delete", "Delete a channel",
        "string", "name", "the channel name", "true",
    },
    {"channel_rename", "Rename a channel",
        "string", "name", "the channel name", "true",
        "string", "new_name", "the new name", "true",
    },
    {"system_create", "Add a system",
        "string", "name", "the system name", "true",
    },
    {"system_delete", "Delete a system",
        "string", "name", "the system name", "true",
    },
    {"planet_create", "Add a planet",
        "string", "name", "the planet name", "true",
        "string", "system", "the parent system name", "true",
    },
    {"planet_delete", "Delete a planet",
        "string", "name", "the planet name", "true",
    },
    {"location_create", "Add a location",
        "string", "name", "the location name", "true",
        "string", "planet", "the parent planet name", "true",
    },
    {"location_delete", "Delete a location",
        "string", "name", "the location name", "true",
    },
};

namespace luxbracer {
    dpp::command_completion_event_t DiscordBot::guild_command_delete_counter(dpp::confirmation_callback_t value) {

        return NULL;
    }

    dpp::command_completion_event_t DiscordBot::update_guild_commands(dpp::confirmation_callback_t value) {
        this->discord_iface->log(dpp::ll_debug, "Guild Command cleanup Callback");
        if ( value.is_error() == true ){
            dpp::error_info err = value.get_error();
            this->discord_iface->log(dpp::ll_error, "Error " + err.message);
        }

        dpp::slashcommand_map map = std::get<dpp::slashcommand_map>(value.value);

        for (auto& it: map) {
            dpp::snowflake id = it.first;
            dpp::slashcommand command = it.second;

            existing_commands.push_back(command.name);

            bool found = std::find(delete_commands.begin(), delete_commands.end(), command.name) != delete_commands.end();

            if (!found) {
                logger->debug("Skipping delete command " + command.name);
                continue;
            }

            this->discord_iface->log(dpp::ll_debug, "Delete " + command.name);

            std::function<void(const dpp::confirmation_callback_t&)> callback =
            std::bind(&DiscordBot::guild_command_delete_counter, this, std::placeholders::_1);
            this->discord_iface->guild_command_delete(id, this->guild_id, callback);
        }

        for (const auto& command : guild_commands) {

            bool found = std::find(existing_commands.begin(), existing_commands.end(), command[0]) != existing_commands.end();

            if (found) {
                logger->debug("Skipping create command " + command[0]);
                continue;
            }

            register_guild_command(command);
        }

        return NULL;
    }

    dpp::command_completion_event_t DiscordBot::delete_global_commands(dpp::confirmation_callback_t value) {
        this->discord_iface->log(dpp::ll_debug, "Global Command cleanup Callback");
        if ( value.is_error() == true ){
            dpp::error_info err = value.get_error();
            this->discord_iface->log(dpp::ll_error, "Error " + err.message);
        }

        dpp::slashcommand_map map = std::get<dpp::slashcommand_map>(value.value);

        for (auto& it: map) {
            dpp::snowflake id = it.first;
            dpp::slashcommand command = it.second;

            bool found = std::find(delete_commands.begin(), delete_commands.end(), command.name) != delete_commands.end();

            if (!found) {
                logger->debug("Skipping command " + command.name);
                continue;
            }

            this->discord_iface->log(dpp::ll_debug, "Delete " + command.name);
            this->discord_iface->global_command_delete(id, NULL);
        }

        return NULL;
    }

    void DiscordBot::register_guild_command(std::vector<std::string> command) {
        dpp::slashcommand cmd = dpp::slashcommand(command[0], command[1], this->discord_iface->me.id);
        dpp::command_option option;

        size_t i = 2;
        while (i < command.size()) {
            if (command[i] == "string") {
                option = dpp::command_option(dpp::co_string, command[i+1], command[i+2], command[i+3] == "true");
                i += 4;
            } else if (command[i] == "string_choice") {
                option = dpp::command_option(dpp::co_string, command[i+1], command[i+2], command[i+3] == "true");
                i += 4;

                while (i < command.size() && command[i] == "choice") {
                    dpp::command_option_choice choice = dpp::command_option_choice(command[i+1], command[i+2]);
                    option.add_choice(choice);
                    i += 3;
                }
            } else {
                this->discord_iface->log(dpp::ll_error, "Error type " + command[i] + " in command " + command[0]);
                break;
            }

            cmd.add_option(option);
        }

        this->discord_iface->log(dpp::ll_debug, "Register " + cmd.name);
        this->discord_iface->guild_command_create(cmd, this->guild_id);

    }

    void DiscordBot::register_guild_commands() {
        for (const auto& command : guild_commands) {
            register_guild_command(command);
        }
    }

    void DiscordBot::slash_commands_init() {
        this->discord_iface->log(dpp::ll_debug, "Register slash commands");

        std::function<void(const dpp::confirmation_callback_t&)> callback =
        std::bind(&DiscordBot::update_guild_commands, this, std::placeholders::_1);
        this->discord_iface->guild_commands_get(guild_id, callback);

        callback = std::bind(&DiscordBot::delete_global_commands, this, std::placeholders::_1);
        this->discord_iface->global_commands_get(callback);
    }

    void DiscordBot::slash_commands_handle(const dpp::slashcommand_t & event) {
	        /* Check which command they ran */
            this->discord_iface->log(dpp::ll_warning, "COMMAND: " + event.command.get_command_name());
	        if (event.command.get_command_name() == "channel_create") {
	            event.reply("command received");

                this->slash_commands_handle_channel_create(event);
	        } else if (event.command.get_command_name() == "channel_delete") {
                event.reply("command received");

                this->slash_commands_handle_channel_delete(event);
            } else if (event.command.get_command_name() == "channel_rename") {
                event.reply("command received");

                this->slash_commands_handle_channel_rename(event);
            } else if (event.command.get_command_name() == "system_create") {
                event.reply("command received");

                this->slash_commands_handle_system_create(event);
            } else if (event.command.get_command_name() == "system_delete") {
                event.reply("command received");

                this->slash_commands_handle_system_delete(event);
            } else if (event.command.get_command_name() == "planet_create") {
                event.reply("command received");

                this->slash_commands_handle_planet_create(event);
            } else if (event.command.get_command_name() == "planet_delete") {
                event.reply("command received");

                this->slash_commands_handle_planet_delete(event);
            } else if (event.command.get_command_name() == "location_create") {
                event.reply("command received");

                this->slash_commands_handle_location_create(event);
            } else if (event.command.get_command_name() == "location_delete") {
                event.reply("command received");

                this->slash_commands_handle_location_delete(event);
            }
	}

    void DiscordBot::system_create(std::string name) {
        if (this->engine->systemAdd(name) == ENGINE_OK) {
            this->channel_create(0, name, dpp::CHANNEL_TEXT);
        }
    }

    void DiscordBot::planet_create(std::string name, std::string system) {

    }

    void DiscordBot::location_create(std::string name, std::string planet) {

    }

    void DiscordBot::system_delete(std::string name) {

    }

    void DiscordBot::planet_delete(std::string name) {

    }

    void DiscordBot::location_delete(std::string name){

    }


    void DiscordBot::slash_commands_handle_system_create(const dpp::slashcommand_t & event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));

        this->system_create(name);
    }

    void DiscordBot::slash_commands_handle_system_delete(const dpp::slashcommand_t & event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));

        this->system_delete(name);
    }

    void DiscordBot::slash_commands_handle_planet_create(const dpp::slashcommand_t & event){
        std::string name = std::get<std::string>(event.get_parameter("name"));
        std::string system = std::get<std::string>(event.get_parameter("system"));

        this->planet_create(name, system);
    }

    void DiscordBot::slash_commands_handle_planet_delete(const dpp::slashcommand_t & event){
        std::string name = std::get<std::string>(event.get_parameter("name"));

        this->planet_delete(name);
    }

    void DiscordBot::slash_commands_handle_location_create(const dpp::slashcommand_t & event){
        std::string name = std::get<std::string>(event.get_parameter("name"));
        std::string planet = std::get<std::string>(event.get_parameter("planet"));

        this->location_create(name, planet);
    }

    void DiscordBot::slash_commands_handle_location_delete(const dpp::slashcommand_t & event){
        std::string name = std::get<std::string>(event.get_parameter("name"));

        this->location_delete(name);
    }

    void DiscordBot::slash_commands_handle_channel_delete(const dpp::slashcommand_t & event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));

        this->channel_delete(name);
        //FIXME parents staff in channel hierarchy
    }

    void DiscordBot::slash_commands_handle_channel_rename(const dpp::slashcommand_t & event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));
        std::string new_name = std::get<std::string>(event.get_parameter("new_name"));

        this->channel_rename(name, new_name);
        //FIXME parents staff in channel hierarchy
    }

    void DiscordBot::slash_commands_handle_channel_create(const dpp::slashcommand_t & event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));

        std::string type = "";
        try {
            type = std::get<std::string>(event.get_parameter("type"));
        } catch (const std::bad_variant_access& e) {
        }

        std::string parent = "";
        try {
            parent = std::get<std::string>(event.get_parameter("parent"));
        } catch (const std::bad_variant_access& e) {
        }

        uint64_t parent_id = 0;

        this->logger->info(name + " # " + type + " # " + parent);

        if (parent != "") {
            std::list<DiscordChannel *> parents = this->guild->channel_get(parent);

            if (parents.size() == 0) {
                this->logger->warn("channel parent not found " + parent);
                return;
            }

            if (parents.size() > 1) {
                this->logger->warn("multiple parents found " + parent);
                return;
            }

            parent_id = parents.front()->id();
        }

        dpp::channel_type chanType = dpp::CHANNEL_TEXT;

        if (type == "Category") {
            chanType = dpp::CHANNEL_CATEGORY;
        }

        this->channel_create(parent_id, name, chanType);
    }
}