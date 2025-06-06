/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_PLANET_EVENT_H_
#define INCLUDE_PLANET_EVENT_H_

#include <string>

#include "ext/json.hpp"
#include "include/lux_types.h"
#include "include/lux_utils.h"

namespace luxbracer {
class PlanetEvent {
 private:
  std::string name;
 public:
  friend void from_json(const nlohmann::json& j, \
                        PlanetEvent & pe) {  // NOLINT(runtime/references)
    j.at("name").get_to(pe.name);
  }
};
}  // namespace luxbracer

#endif  // INCLUDE_PLANET_EVENT_H_
