/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_ENGINE_H_
#define INCLUDE_ENGINE_H_

#include <map>
#include <string>

#include "include/lux_types.h"
#include "include/discord_postman.h"
#include "include/system.h"
#include "ext/json.hpp"

namespace luxbracer {
class Engine {
 private:
  uint64_t index = 0;
  uint64_t date = 0;

  std::map<std::string, System> system_map;

  Logger * logger = NULL;             // no_serial
  DiscordPostman * postman = NULL;    // no_serial

  void execute_player_actions();
  void date_advance();
  void systems_advance();
  void planets_advanve();

 public:
  Engine(void);
  explicit Engine(Logger * l);

  void init(void);

  void create_channels(void);
  void set_postman(DiscordPostman * p);

  uint64_t next_id(void);

  engineError systemAdd(const System & system);
  engineError systemAdd(const std::string & name);
  engineError planetAdd(const std::string & name, const std::string & system);

  void setLogger(Logger * l);
  void setPostman(const DiscordPostman & p);

  System * systemGet(const std::string & name);
  Planet * planetGet(const std::string & name, const std::string & system);
  engineError planetRemove(const std::string & name,
                           const std::string & system);

  friend void to_json(nlohmann::json& j, \
                      const Engine& e);  // NOLINT(runtime/references)
  friend void from_json(const nlohmann::json& j, \
                        Engine & e);  // NOLINT(runtime/references)

  void execute_game_loop(void);
};

inline
void to_json(nlohmann::json& j, \
             const Engine & e) {  // NOLINT(runtime/references)
  j = nlohmann::json{
      {"index", e.index},
      {"system_map", e.system_map},
      {"date", e.date},
  };
}

inline
void from_json(const nlohmann::json& j,
                Engine & e) {  // NOLINT(runtime/references)
  j.at("index").get_to(e.index);
  j.at("system_map").get_to(e.system_map);
  j.at("date").get_to(e.date);
}
}  // namespace luxbracer

#endif  // INCLUDE_ENGINE_H_
