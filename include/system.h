/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_SYSTEM_H_
#define INCLUDE_SYSTEM_H_

#include <cstdint>
#include <string>
#include <map>
#include <vector>

#include "ext/json.hpp"
#include "include/lux_types.h"
#include "include/planet.h"
#include "include/lux_logger.h"

namespace luxbracer {
class System {
 private:
  Logger * logger = NULL;  // no_serial
  std::string    name;
  uint64_t        id;

  std::map<std::string, Planet> planet_map;

 public:
  System(void) = default;
  System(const std::string& name, uint64_t id);

  std::string getName(void) const;

  uint64_t getId(void) const;

  engineError planetAdd(Planet planet);
  engineError planetRemove(const std::string & name);
  Planet * planetGet(const std::string & name);

  std::vector<std::string> get_planet_names(void);

  void setLogger(Logger * l);

  friend void to_json(nlohmann::json& j, \
                      const System & s);  // NOLINT(runtime/references)
  friend void from_json(const nlohmann::json& j, \
                        System & s);  // NOLINT(runtime/references)
};

inline
void to_json(nlohmann::json& j, \
             const System & s) {  // NOLINT(runtime/references)
  j = nlohmann::json{
      {"name", s.name},
      {"id", s.id},
      {"planet_map", s.planet_map}
  };
}

inline
void from_json(const nlohmann::json& j,
                System & s) {  // NOLINT(runtime/references)
  j.at("name").get_to(s.name);
  j.at("id").get_to(s.id);
  j.at("planet_map").get_to(s.planet_map);
}
}  // namespace luxbracer

#endif  // INCLUDE_SYSTEM_H_
