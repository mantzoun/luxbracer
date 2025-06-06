/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_DISCORD_GUILD_H_
#define INCLUDE_DISCORD_GUILD_H_

#include <string>
#include <list>

#include "dpp/dpp.h"
#include "include/discord_channel.h"
#include "include/lux_logger.h"

namespace luxbracer {
class DiscordGuild {
 private:
  dpp::snowflake  _id;
  std::list<DiscordChannel> channels;
  Logger * logger = NULL;

  int verify_and_add_channel(const DiscordChannel& channel);
  int verify_and_delete_channel(const DiscordChannel& channel);
  int verify_and_update_channel(const DiscordChannel& channel);

 public:
  explicit DiscordGuild(const dpp::snowflake id);

  dpp::snowflake id(void) const;

  void set_logger(Logger * logger);

  int channel_add(const dpp::channel_create_t & channel);
  int channel_add(const dpp::channel & channel);
  int channel_delete(const dpp::channel_delete_t & channel);
  int channel_update(const dpp::channel_update_t & channel);

  std::list<DiscordChannel *> channel_get(const std::string & name,
                                            dpp::snowflake channel_id = 0,
                                            dpp::snowflake parent_id = 0);
  DiscordChannel * channel_get(const std::string & name,
                               const std::string & parent);
  std::list<DiscordChannel> channel_get_all(void) const;
  DiscordChannel * channel_get_by_id(dpp::snowflake id);
};
}  // namespace luxbracer

#endif  // INCLUDE_DISCORD_GUILD_H_
