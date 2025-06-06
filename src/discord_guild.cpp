/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "include/discord_guild.h"

namespace luxbracer {
DiscordGuild::DiscordGuild(const dpp::snowflake id)
    : _id(id) {
}

void DiscordGuild::set_logger(Logger * logger) {
  this->logger = logger;
}

dpp::snowflake  DiscordGuild::id(void) const {
  return _id;
}

int DiscordGuild::verify_and_add_channel(const DiscordChannel& channel) {
  if (std::any_of(channels.begin(), channels.end(), [&](const auto& item) {
    return item == channel;
  })) {
    // channel exists
    logger->warn("Channel already exists : " + channel.name() +
                 "," + std::to_string(channel.id()) +
                 "," + std::to_string(channel.parent()));
    return -1;
  }

  channels.push_back(channel);
  logger->debug("Added channel : " + channel.name() +
                "," + std::to_string(channel.id()) +
                "," + std::to_string(channel.parent()));
  return 0;
}

int DiscordGuild::verify_and_delete_channel(const DiscordChannel& channel) {
  for (auto it = channels.begin(); it != channels.end(); ) {
    if (*it == channel) {
      channels.erase(it);
      logger->debug("Deleted channel : " + channel.name() +
                    "," + std::to_string(channel.id()) +
                    "," + std::to_string(channel.parent()));
      return 0;
    }
    it++;
  }

  logger->warn("Could not delete channel : " + channel.name() +
               "," + std::to_string(channel.id()) +
               "," + std::to_string(channel.parent()));
  return -1;
}

int DiscordGuild::verify_and_update_channel(const DiscordChannel& channel) {
  for (auto it = channels.begin(); it != channels.end(); ) {
    if (it->id() == channel.id()) {
      it->set_name(channel.name());
      it->set_parent(channel.parent());
      logger->debug("Updated channel : " + channel.name() +
                    "," + std::to_string(channel.id()) +
                    "," + std::to_string(channel.parent()));
      return 0;
    }
    it++;
  }

  logger->warn("Could not update channel : " + channel.name() +
               "," + std::to_string(channel.id()) +
               "," + std::to_string(channel.parent()));
  return -1;
}

int DiscordGuild::channel_add(const dpp::channel & channel) {
  return verify_and_add_channel(DiscordChannel(channel.id,
                                    channel.parent_id, channel.name));
}

int DiscordGuild::channel_add(const dpp::channel_create_t & channel) {
  return verify_and_add_channel(DiscordChannel(channel.created.id,
                                                 channel.created.parent_id,
                                                 channel.created.name));
}

int DiscordGuild::channel_update(const dpp::channel_update_t & channel) {
  return verify_and_update_channel(DiscordChannel(channel.updated.id,
                                                    channel.updated.parent_id,
                                                    channel.updated.name));
}

int DiscordGuild::channel_delete(const dpp::channel_delete_t & channel) {
  return verify_and_delete_channel(DiscordChannel(channel.deleted.id,
                                                    channel.deleted.parent_id,
                                                    channel.deleted.name));
}

DiscordChannel * DiscordGuild::channel_get(const std::string & name,
                                           const std::string & parent) {
  DiscordChannel * result = NULL;
  dpp::snowflake parent_id = 0;

  if (parent != "") {
    DiscordChannel * parent_channel = this->channel_get(parent, "");
    if (parent_channel == NULL) {
      logger->error("Could not find parent channel");
      return NULL;
    }

    parent_id = parent_channel->id();
  }

  for (DiscordChannel& channel : channels) {
    if (channel.name() == name &&
      (parent_id == 0 || channel.parent() == parent_id)) {
      if (result == NULL) {
        result = &channel;
      } else {
        this->logger->error("Found duplicate channel " + name);
      }
    }
  }

  return result;
}

std::list<DiscordChannel *> DiscordGuild::channel_get(const std::string& name,
                                                    dpp::snowflake channel_id,
                                                    dpp::snowflake parent_id) {
  std::list<DiscordChannel *> result;

  for (DiscordChannel& channel : channels) {
    if (channel.name() == name &&
                (parent_id == 0 || channel.parent() == parent_id) &&
                (channel_id == 0 || channel.id() == channel_id)) {
      logger->debug("Add search result channel : " + channel.name() +
                    "," + std::to_string(channel.id()) +
                    "," + std::to_string(channel.parent()));
      result.push_back(&channel);
    }
  }

  return result;
}

DiscordChannel * DiscordGuild::channel_get_by_id(dpp::snowflake id) {
  if (id == 0) {
    return NULL;
  }

  auto it = std::find_if(channels.begin(), channels.end(),
  [&](const auto& channel) { return id == channel.id(); });

  if (it != channels.end()) {
    return &(*it);
  }

  return NULL;
}
}  // namespace luxbracer
