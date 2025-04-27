/*
 * luxbracer_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "discord_bot.h"

namespace luxbracer {
    void register_guild_commands();

    DiscordBot::DiscordBot(void)
    {
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

    void DiscordBot::channel_rename(std::string name, std::string new_name) {
        if (this->channels_list.contains(name)) {
            dpp::snowflake id = channels_list[name];
            dpp::channel channel;

            channel.id = id;
            channel.set_name(new_name);
            this->discord_iface->channel_edit(channel, NULL);

            auto entry = channels_list.extract(name);
            entry.key() = new_name;
            channels_list.insert(std::move(entry));

        } else {
            this->discord_iface->log(dpp::ll_debug, "Invalid channel rename: " + name);
        }
    }
    dpp::channel ch;

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
