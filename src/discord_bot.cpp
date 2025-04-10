/*
 * luxbracer_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include <stdio.h>
#include <map>
#include <mutex>

#include "discord_bot.h"

#define NEW_CMD_OPTION(OBJ, TYPE, NAME, DESC, OBLIG, ...) \
OBJ->add_option(dpp::command_option(TYPE, NAME, DESC, OBLIG)  __VA_ARGS__)

#define NEW_CMD_CHOICE(OPT_NAME, OPT_VALUE) \
.add_choice(dpp::command_option_choice(OPT_NAME, OPT_VALUE))

namespace luxbracer {
    void register_guild_commands();

    DiscordBot::DiscordBot(void)
    {
    }

    // ===================================================================
    //                          SLASH COMMANDS
    // ===================================================================
    int total_commands = 0;
    std::mutex myMutex;

    dpp::command_completion_event_t DiscordBot::guild_command_delete_counter(dpp::confirmation_callback_t value) {
        std::lock_guard<std::mutex> lock(myMutex); // Locks the mutex
        static int calls = 0;

        this->discord_iface->log(dpp::ll_debug, "Guild Command Delete Callback");
        if ( value.is_error() == true ){
            dpp::error_info err = value.get_error();
            this->discord_iface->log(dpp::ll_error, "Error " + err.message);
        }

        if (++calls == total_commands) {
            //all deleted, register now
            register_guild_commands();
        }

        return NULL;
    }

    dpp::command_completion_event_t DiscordBot::delete_guild_commands(dpp::confirmation_callback_t value) {
        this->discord_iface->log(dpp::ll_debug, "Guild Command cleanup Callback");
        if ( value.is_error() == true ){
            dpp::error_info err = value.get_error();
            this->discord_iface->log(dpp::ll_error, "Error " + err.message);
        }

        dpp::slashcommand_map map = std::get<dpp::slashcommand_map>(value.value);

        for (auto& it: map) {
            total_commands++;
            dpp::snowflake id = it.first;
            dpp::slashcommand command = it.second;

            this->discord_iface->log(dpp::ll_debug, "Delete " + command.name);

            std::function<void(const dpp::confirmation_callback_t&)> callback =
            std::bind(&DiscordBot::guild_command_delete_counter, this, std::placeholders::_1);
            this->discord_iface->guild_command_delete(id, this->guild_id, callback);
        }

        if (total_commands == 0) {
            register_guild_commands();
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

            this->discord_iface->log(dpp::ll_debug, "Delete " + command.name);
            this->discord_iface->global_command_delete(id, NULL);
        }

        return NULL;
    }

    void DiscordBot::register_guild_commands() {
        dpp::slashcommand * cmd = new dpp::slashcommand("channel_create", "Create a new channel", this->discord_iface->me.id);
        NEW_CMD_OPTION(cmd, dpp::co_string, "name", "the channel name", true);
        NEW_CMD_OPTION(cmd, dpp::co_string, "parent", "the channel parent", false);
        NEW_CMD_OPTION(cmd, dpp::co_string, "type", "the channel type", false,
            NEW_CMD_CHOICE("Text", "TextChannel")
            NEW_CMD_CHOICE("Category", "Category"));

        this->discord_iface->log(dpp::ll_debug, "Register " + cmd->name);
        this->discord_iface->guild_command_create(*cmd, this->guild_id);
        delete cmd;

        cmd = new dpp::slashcommand("channel_delete", "Delete a channel", this->discord_iface->me.id);
        NEW_CMD_OPTION(cmd, dpp::co_string, "name", "the channel name", true);

        this->discord_iface->log(dpp::ll_debug, "Register " + cmd->name);
        this->discord_iface->guild_command_create(*cmd, this->guild_id);
        delete cmd;

        cmd = new dpp::slashcommand("channel_rename", "Rename a channel", this->discord_iface->me.id);
        NEW_CMD_OPTION(cmd, dpp::co_string, "name", "the channel name", true);
        NEW_CMD_OPTION(cmd, dpp::co_string, "new_name", "the new name", false);

        this->discord_iface->log(dpp::ll_debug, "Register " + cmd->name);
        this->discord_iface->guild_command_create(*cmd, this->guild_id);
        delete cmd;
    }

    void DiscordBot::slash_commands_init() {
        this->discord_iface->log(dpp::ll_debug, "Register slash commands");

        std::function<void(const dpp::confirmation_callback_t&)> callback =
        std::bind(&DiscordBot::delete_guild_commands, this, std::placeholders::_1);
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
            }
	}

    void DiscordBot::slash_commands_handle_channel_delete(const dpp::slashcommand_t & event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));

        this->channel_delete(name);
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
    // ===================================================================
    //                          MESSAGES
    // ===================================================================
    void DiscordBot::post_message(std::string channel, std::string text) {
        dpp::message message;

        message.channel_id = channels_list[channel];
        message.guild_id = this->guild_id;
        message.content = text;
        this->discord_iface->message_create(message);
    }

    // ===================================================================
    //                          CHANNELS
    // ===================================================================
    void DiscordBot::channel_create(dpp::snowflake parent_id, std::string name, dpp::channel_type chanType) {
        dpp::channel chan;

        chan.name = name;
        chan.guild_id = this->guild_id;
        chan.set_type(chanType);

        if (parent_id != 0) {
            chan.parent_id = parent_id;
        }

        this->discord_iface->channel_create(chan);
    }

    void DiscordBot::channel_delete(std::string name) {
        if (this->channels_list.contains(name)) {
            this->discord_iface->channel_delete(channels_list[name], NULL);
        } else {
            this->discord_iface->log(dpp::ll_debug, "Invalid channel delete: " + name);
        }
    }

    //dpp::command_completion_event_t existing_devices(dpp::confirmation_callback_t value)
    //{
    //    this->discord_iface->log(dpp::ll_debug, "devices init Callback");
    //    if ( value.is_error() == true ){
    //        dpp::error_info err = value.get_error();
    //        this->discord_iface->log(dpp::ll_error, "Error " + err.message);
    //    }
    //
    //    dpp::message_map map = std::get<dpp::message_map>(value.value);
    //
    //    for (auto& it: map) {
    //        std::string device_name;
    //        dpp::message m = it.second;
    //
    //        if (m.channel_id == channel_map["devices"]){
    //            size_t fs = m.content.find(" ");
    //
    //            if (fs == std::string::npos){
    //                device_name = m.content;
    //            } else {
    //                device_name = m.content.substr(0, fs);
    //            }
    //
    //            this->discord_iface->log(dpp::ll_debug, "found message for " + device_name);
    //            device_map[device_name] = m.id;
    //        }
    //    }
    //
    //    return NULL;
    //}

    // dpp::command_completion_event_t my_message_cb(dpp::confirmation_callback_t value)
    // {
    //    this->discord_iface->log(dpp::ll_debug, "message Callback");
    //    if ( value.is_error() == true ){
    //        dpp::error_info err = value.get_error();
    //        this->discord_iface->log(dpp::ll_error, "Error " + err.message);
    //    }

    //    dpp::message m = std::get<dpp::message>(value.value);

    //    return NULL;
    // }

    dpp::command_completion_event_t DiscordBot::channels_create_cb(dpp::confirmation_callback_t value) {
        dpp::channel channel = std::get<dpp::channel>(value.value);

        this->discord_iface->log(dpp::ll_debug, "Created channel " + channel.name + " with id " + std::to_string(channel.id));
        this->channels_list[channel.name] = channel.id;

        return NULL;
    }

    dpp::command_completion_event_t  DiscordBot::channels_get_callback(dpp::confirmation_callback_t value)
    {
        this->discord_iface->log(dpp::ll_debug, "channels Callback");

        if ( value.is_error() == true ){
            dpp::error_info err = value.get_error();
            this->discord_iface->log(dpp::ll_error, "Error " + err.message);
        }

        dpp::channel_map channelmap = std::get<dpp::channel_map>(value.value);

        dpp::snowflake id;
        dpp::channel c;

        dpp::snowflake parent_id;

        std::list default_channels = {"syslog"};

        for (auto& it: channelmap) {
            id = it.first;
            c = it.second;

            parent_id = c.parent_id;

            this->discord_iface->log(dpp::ll_debug, "" + c.name + " " + c.id.str() + " " + c.parent_id.str());
            this->channel_added_callback(c.name, c.id.str(), c.parent_id.str());

            this->discord_iface->log(dpp::ll_debug, "Found channel " + c.name + " with id " + std::to_string(id));
            this->channels_list[c.name] = id;

            //    if (c.name == "devices") {
            //        this->discord_iface->messages_get(id, 0, 0, 0, 0, &existing_devices);
            //    }
        }

        for (std::string name : default_channels) {
            if (! channels_list.contains(name)) {
                this->discord_iface->log(dpp::ll_debug, "Init Create channel " + name);

                dpp::channel chan;

                chan.name = name;
                chan.guild_id = this->guild_id;
                chan.set_type(dpp::CHANNEL_TEXT);

                std::function<void(const dpp::confirmation_callback_t&)> callback =
                std::bind(&DiscordBot::channels_create_cb, this, std::placeholders::_1);
                this->discord_iface->channel_create(chan, callback);
            }
        }

        // if (cat == 0){
        //     std::string n = "Sol";
        //     this->discord_iface->log(dpp::ll_info, "create category");
        //     luxbracer_discord_this->discord_iface->category_create(gid, cat, n);
        // } else if (chan == 0){
        //     std::string n = "test";
        //     this->discord_iface->log(dpp::ll_info, "create channel, parent " + std::to_string(cat));
        //     luxbracer_discord_this->discord_iface->channel_create(gid, cat, n);
        // }

        // dpp::message m;
        // m.channel_id = channel_map["syslog"];
        // m.content    = "Bot conneced, bot id " + luxbracer_discord_this->discord_iface->bot_id();
        // this->discord_iface->message_create(m, &my_message_cb);

        return NULL;
    }

    void DiscordBot::channel_added_callback(std::string name, dpp::snowflake channel_id, dpp::snowflake parent_id) {
        this->guild->channel_add(new DiscordChannel(channel_id, parent_id, name));
    }

    void DiscordBot::channel_deleted_callback(std::string name, dpp::snowflake channel_id, dpp::snowflake parent_id) {
        this->guild->channel_delete(channel_id, parent_id, name);
    }

    dpp::command_completion_event_t DiscordBot::user_get_guilds_callback(dpp::confirmation_callback_t value)
    {
        this->discord_iface->log(dpp::ll_debug, "Guilds Callback");
        if ( value.is_error() == true ){
            dpp::error_info err = value.get_error();
            this->discord_iface->log(dpp::ll_error, "Error " + err.message);
        }

        dpp::guild_map guildmap = std::get<dpp::guild_map>(value.value);

        if (guildmap.size() > 1) {
            this->discord_iface->log(dpp::ll_error, "Error, too many guilds ");
            return NULL;
        }

        dpp::snowflake id;
        dpp::guild g;

        for (auto& it: guildmap) {
            id = it.first;
            g = it.second;

            this->initialize_guild(id);
            this->guild_id = id;

            this->slash_commands_init();

            std::function<void(const dpp::confirmation_callback_t&)> callback =
            std::bind(&DiscordBot::channels_get_callback, this, std::placeholders::_1);

            this->discord_iface->channels_get(id, callback);
       }

        return NULL;
    }

    void DiscordBot::initialize_guild(dpp::snowflake id) {
        this->guild = new DiscordGuild(id);
    }

    //void luxbracer::DiscordBot::message_cb(luxbracer::callback_msg * msg)
    //{
    //    dpp::message m, s;
    //    dpp::component c;
    //    dpp::component ar;
    //
    //    std::string content = msg->content;
    //    std::string channel = msg->channel;
    //
    //    this->discord_iface->log(dpp::ll_debug, "received message " + std::to_string(msg->type) + " : " + content + " : " + channel);
    //    switch(msg->type){
    //        case CDB_MSG_DISC_MQTT_DEV_ADD:
    //            if (device_map.find(content) == device_map.end()){
    //                this->discord_iface->log(dpp::ll_debug, "message not found for " + content);
    //                m.channel_id = channel_map["devices"];
    //                m.content    = content;
    //
    //                ar.set_type(dpp::cot_action_row);
    //
    //                c.set_style(dpp::cos_success);
    //                c.set_label("ON");
    //                c.set_id(content + "#ON");
    //                c.set_type(dpp::cot_button);
    //
    //                ar.add_component(c);
    //
    //                c.set_style(dpp::cos_danger);
    //                c.set_label("OFF");
    //                c.set_id(content + "#OFF");
    //                c.set_type(dpp::cot_button);
    //
    //                ar.add_component(c);
    //
    //                m.add_component(ar);
    //
    //                this->discord_iface->message_create(m, &my_message_cb);
    //            } else {
    //                this->discord_iface->log(dpp::ll_debug, "message found for " + content + ". Skipping");
    //            }
    //            break;
    //        case CDB_MSG_DISC_MQTT_DEV_STATUS_ON:
    //        case CDB_MSG_DISC_MQTT_DEV_STATUS_OFF:
    //            if (device_map.find(content) == device_map.end()){
    //                this->discord_iface->log(dpp::ll_warning, "status message not found for " + content);
    //                return;
    //            }
    //
    //            m = this->discord_iface->message_get_sync(device_map[content], channel_map["devices"]);
    //            if (msg->type == CDB_MSG_DISC_MQTT_DEV_STATUS_ON){
    //                m.content    = content + " is ON";
    //            } else if (msg->type == CDB_MSG_DISC_MQTT_DEV_STATUS_OFF){
    //                m.content    = content + " is OFF";
    //            }
    //
    //            s.channel_id = channel_map["syslog"];
    //            s.content    = m.content;
    //
    //            this->discord_iface->message_edit(m, &my_message_cb);
    //            this->discord_iface->message_create(s, &my_message_cb);
    //            break;
    //        case CDB_MSG_DISC_POST_FILE:
    //            m.channel_id = channel_map[channel];
    //            m.add_file(content.substr(content.find_last_of("\\/"), content.size()),
    //                       dpp::utility::read_file(content));
    //
    //            this->discord_iface->message_create(m, &my_message_cb);
    //            break;
    //        case CDB_MSG_DISC_POST_MESSAGE:
    //            m.channel_id = channel_map[channel];
    //            m.content    = content;
    //
    //            this->discord_iface->message_create(m, &my_message_cb);
    //            break;
    //        default:
    //            break;
    //    }
    //}

    void DiscordBot::init(std::string token, std::string id)
    {
        this->discord_iface = new dpp::cluster(token);

        //luxbracer_discord_bot = this;

        this->bot_id = bot_id;

        // Use our own logger for output consistency
        this->discord_iface->on_log([this](const dpp::log_t & event) {
       switch (event.severity) {
           case dpp::ll_trace:
           case dpp::ll_debug:
               this->logger->debug("Discord Bot: " + event.message);
               break;
           case dpp::ll_info:
               this->logger->info("Discord Bot: " +  event.message);
               break;
           case dpp::ll_warning:
               this->logger->warn("Discord Bot: " + event.message);
               break;
           case dpp::ll_error:
           case dpp::ll_critical:
           default:
               this->logger->error("Discord Bot: " + event.message);
               break;
           }
       });


    //    this->discord_iface->on_button_click([this](const dpp::button_click_t & event) {
    //        /* Button clicks are still interactions, and must be replied to in some form to
    //         * prevent the "this interaction has failed" message from Discord to the user.
    //         */
    //        event.reply();
    //
    //        char dev_id[50];
    //        char * tmp;
    //        int index = 0;
    //        luxbracer::callback_msg msg = {CDB_MSG_MAX, "N/A"};
    //
    //        this->logger->debug("parse message  " + event.custom_id);
    //
    //        strncpy(dev_id, event.custom_id.c_str(), sizeof(dev_id));
    //        tmp = strtok(dev_id, "#");
    //
    //        while (tmp != NULL)
    //        {
    //            switch(index++){
    //                case 0:
    //                    msg.content = tmp;
    //                    this->logger->debug("message is " + msg.content);
    //                    break;
    //                case 1:
    //                    if (strcmp(tmp, "ON") == 0) {
    //                        msg.type = CDB_MSG_MQTT_HANDLER_TURN_DEVICE_ON;
    //                    } else if (strcmp(tmp, "OFF") == 0) {
    //                        msg.type = CDB_MSG_MQTT_HANDLER_TURN_DEVICE_OFF;
    //                    }
    //
    //                    this->logger->debug("type is " + std::to_string(msg.type));
    //                    break;
    //                default:
    //                    this->logger->error("Unexpected number of arguments : " + event.custom_id);
    //                    break;
    //            }
    //
    //            tmp = strtok (NULL, "#");
    //        }
    //
    //        this->m_handler->message_cb(&msg);
    //    });

        this->discord_iface->on_channel_create([this](const dpp::channel_create_t & event) {
            this->channel_added_callback(event.created.name,
                              event.created.id,
                              event.created.parent_id);

            this->channels_list[event.created.name] = event.created.id;
        });

        this->discord_iface->on_channel_delete([this](const dpp::channel_delete_t & event) {
            this->channel_deleted_callback(event.deleted.name,
                              event.deleted.id,
                              event.deleted.parent_id);

            this->channels_list.erase(event.deleted.name);
        });

        this->discord_iface->on_slashcommand([this](const dpp::slashcommand_t & event) {
            this->slash_commands_handle(event);
        });

        this->discord_iface->on_ready([this](const dpp::ready_t& event) {
            std::function<void(const dpp::confirmation_callback_t&)> callback =
            std::bind(&DiscordBot::user_get_guilds_callback, this, std::placeholders::_1);

            this->discord_iface->current_user_get_guilds(callback);
        });

        this->discord_iface->start(dpp::st_return);
    }

    void DiscordBot::set_logger(Logger * l)
    {
    this->logger = l;
    }

    std::string DiscordBot::bot_id_get(void)
    {
        return this->bot_id;
    }
}
