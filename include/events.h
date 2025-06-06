/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_EVENTS_H_
#define INCLUDE_EVENTS_H_

#include "ext/json.hpp"
#include "include/lux_types.h"
#include "include/lux_utils.h"
#include "include/planet_event.h"
#include "include/system_event.h"
#include "include/planet.h"
#include "include/system.h"

namespace luxbracer {
class Events {
 private:
  Logger * logger;  // no_serial

  PlanetEvents    planet_events;
  SystemEvents    system_events;
 public:
  explicit Events(Logger * logger);

  void apply_events(Planet * planet);
  void process_events(Planet * planet);

  void apply_events(System * system);
  void process_events(System * system);

  friend void from_json(const nlohmann::json& j, \
                            Events & e);  // NOLINT(runtime/references)
};

inline
void from_json(const nlohmann::json& j,
                    Events & e) {  // NOLINT(runtime/references)
  j.at("planet_events").get_to(e.planet_events);
  j.at("system_events").get_to(e.system_events);
}
}  // namespace luxbracer

#endif  // INCLUDE_EVENTS_H_
