/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_DISCORD_POSTMAN_H_
#define INCLUDE_DISCORD_POSTMAN_H_

#include <string>

namespace luxbracer {
class DiscordPostman {
 public:
  virtual ~DiscordPostman() = default;

  virtual void post(const std::string & message,
                    const std::string & channel) = 0;
  virtual void add_system_channel(const std::string & system_name) = 0;
  virtual void add_planet_channel(const std::string & planet_name,
                                  const std::string & system_name) = 0;
};
}  // namespace luxbracer

#endif  // INCLUDE_DISCORD_POSTMAN_H_
