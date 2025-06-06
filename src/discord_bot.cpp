/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * luxbracer_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "include/discord_bot.h"

static std::vector<std::string> non_engine_categories = {"Text Channels"};

namespace luxbracer {
void register_guild_commands();

DiscordBot::DiscordBot(void) {
}

// ===================================================================
//                          INTERFACE
// ===================================================================
void DiscordBot::post(const std::string & message,
                      const std::string & channel) {
  this->post_message(channel, message);
}

void DiscordBot::add_system_channel(const std::string & system_name) {
  if (this->guild->channel_get(system_name, "") == NULL) {
    this->channel_create(0, system_name, dpp::CHANNEL_CATEGORY);
    usleep(1 * 1000 * 1000);
  }
}

void DiscordBot::add_planet_channel(const std::string & planet_name,
                                    const std::string & system_name) {
DiscordChannel * system_channel = this->guild->channel_get(system_name, "");

if (system_channel == NULL) {
  this->logger->error("Missing system channel" + system_name);
  return;
}

if (this->guild->channel_get(planet_name, system_name) == NULL) {
  this->channel_create(system_channel->id(), planet_name, dpp::CHANNEL_TEXT);
  usleep(1 * 1000 * 1000);
}
}

// ===================================================================
//                          MESSAGES
// ===================================================================
void DiscordBot::post_message(const std::string & channel_name,
                              const std::string & text) {
  dpp::message message;

  std::list<DiscordChannel *> channels = this->guild->channel_get(channel_name);

  if (channels.size() == 0) {
    logger->error("No channels found to post : " + channel_name);
    return;
  }

  for (DiscordChannel * channel : channels) {
    message.channel_id = channel->id();
    message.guild_id = this->guild_id;
    message.content = text;
    logger->debug("Post to channel : " + channel->name() + "," + std::to_string(channel->id()) +
                  "," + std::to_string(channel->parent()));
    this->discord_iface->message_create(message);
  }
}

// ===================================================================
//                          CHANNELS
// ===================================================================
void DiscordBot::channel_create(dpp::snowflake parent_id,
                                const std::string & name,
                                dpp::channel_type chanType) {
  dpp::channel chan;

  chan.name = name;
  chan.guild_id = this->guild_id;
  chan.set_type(chanType);

  if (parent_id != 0) {
    chan.parent_id = parent_id;
  }

  this->discord_iface->channel_create(chan);
}

void DiscordBot::channel_delete(const std::string & name) {
  std::list<DiscordChannel *> channels = this->guild->channel_get(name);

  if (channels.size() != 1) {
    logger->warn("Number of channels to delete not 1 : " +
                    std::to_string(channels.size()));
    return;
  }

  DiscordChannel * discord_channel = channels.front();
  this->discord_iface->channel_delete(discord_channel->id(), NULL);
}

void DiscordBot::channel_rename(const std::string & name,
                                std::string new_name) {
  std::list<DiscordChannel *> channels = this->guild->channel_get(name);

  if (channels.size() != 1) {
    logger->warn("Number of channels to rename not 1 : " +
                    std::to_string(channels.size()));
    return;
  }

  DiscordChannel * discord_channel = channels.front();

  dpp::channel channel;

  channel.id = discord_channel->id();
  channel.set_name(new_name);
  this->discord_iface->channel_edit(channel, NULL);
}

dpp_event_t DiscordBot::channels_get_callback(dpp_cb_t value) {
  this->logger->debug("channels Callback");

  if ( value.is_error() == true ) {
    dpp::error_info err = value.get_error();
    logger->error("Error " + err.message);
  }

  dpp::channel_map channelmap = std::get<dpp::channel_map>(value.value);

  dpp::snowflake id;
  dpp::channel channel;

  dpp::snowflake parent_id;

  std::list default_channels = {"syslog"};

  for (const auto& it : channelmap) {
    id = it.first;
    channel = it.second;

    parent_id = channel.parent_id;

    this->logger->debug("" + channel.name + " " +
                             channel.id.str() + " " +
                             channel.parent_id.str());
    this->guild->channel_add(channel);

    this->logger->debug("Found channel " + channel.name +
                        " with id " + std::to_string(id));
#ifdef ENGINE_INIT_FROM_DISCORD
      if (channel.parent_id == 0 &&
              channel.get_type() == dpp::CHANNEL_CATEGORY) {
        if (std::find(non_engine_categories.begin(),
                      non_engine_categories.end(),
                      channel.name) != non_engine_categories.end()) {
          // channel found in ignore list
          continue;
        }
        this->logger->debug("Adding system " + channel.name + " to engine");
        this->engine->systemAdd(channel.name);
      }
#endif /* ENGINE_INIT_FROM_DISCORD */
    }

#ifdef ENGINE_INIT_FROM_DISCORD
  // Added systems in first pass, now add planets
  for (const auto& it : channelmap) {
    id = it.first;
    channel = it.second;

    parent_id = channel.parent_id;

    if (parent_id != 0) {
      DiscordChannel * parent = this->guild->channel_get_by_id(parent_id);

      if (parent == NULL) {
        this->logger->warn("Could not fidn parent of " + channel.name);
        continue;
      }
      this->logger->debug("Adding planet " + channel.name +
                          " to system " + parent->name());
      this->engine->planetAdd(channel.name, parent->name());
    }
  }
#endif /* ENGINE_INIT_FROM_DISCORD */

  for (std::string ch_name : default_channels) {
    std::list<DiscordChannel *> channels = this->guild->channel_get(ch_name);

    if (channels.size() > 0) {
      continue;
    }

    this->logger->debug("Init Create channel " + ch_name);

    dpp::channel chan;

    chan.name = ch_name;
    chan.guild_id = this->guild_id;
    chan.set_type(dpp::CHANNEL_TEXT);

    // std::function<void(const dpp_cb_t&)> callback =
    // std::bind(&DiscordBot::channels_create_cb, this, std::placeholders::_1);
    this->discord_iface->channel_create(chan);  //, callback);
  }

  this->init_complete = true;

  return NULL;
}

dpp_event_t DiscordBot::user_get_guilds_callback(dpp_cb_t value) {
  this->logger->debug("Guilds Callback");
  if (value.is_error() == true) {
    dpp::error_info err = value.get_error();
    this->logger->error("Error " + err.message);
  }

  dpp::guild_map guildmap = std::get<dpp::guild_map>(value.value);

  if (guildmap.size() > 1) {
    this->logger->error("Error, too many guilds ");
    return NULL;
  }

  dpp::snowflake id;
  dpp::guild g;

  for (const auto& it : guildmap) {
    id = it.first;
    g = it.second;

    this->initialize_guild(id);
    this->guild_id = id;

    this->slash_commands_init();

    std::function<void(const dpp_cb_t&)> callback =
    std::bind(&DiscordBot::channels_get_callback, this, std::placeholders::_1);

    this->discord_iface->channels_get(id, callback);
  }

  return NULL;
}

void DiscordBot::initialize_guild(dpp::snowflake id) {
  this->guild = new DiscordGuild(id);
  this->guild->set_logger(logger);
}

void DiscordBot::init(std::string token, const std::string & id) {
  this->discord_iface = new dpp::cluster(token);

  // luxbracer_discord_bot = this;

  this->bot_id = id;

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

  this->discord_iface->on_channel_create(
          [this](const dpp::channel_create_t & channel) {
    this->guild->channel_add(channel);
  });

  this->discord_iface->on_channel_delete(
          [this](const dpp::channel_delete_t & channel) {
    this->guild->channel_delete(channel);
  });

  this->discord_iface->on_channel_update(
          [this](const dpp::channel_update_t & channel) {
    this->guild->channel_update(channel);
  });

  this->discord_iface->on_slashcommand(
          [this](const dpp::slashcommand_t & event) {
    this->slash_commands_handle(event);
  });

  this->discord_iface->on_ready([this](const dpp::ready_t& event) {
    std::function<void(const dpp_cb_t&)> callback =
    std::bind(&DiscordBot::user_get_guilds_callback,
              this, std::placeholders::_1);

    this->discord_iface->current_user_get_guilds(callback);
  });

  this->discord_iface->start(dpp::st_return);
}

void DiscordBot::set_logger(Logger * l) {
  this->logger = l;
}

void DiscordBot::set_engine(Engine * e) {
  this->engine = e;
}
}  // namespace luxbracer
