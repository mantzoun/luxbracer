/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_SYSTEM_EVENT_H_
#define INCLUDE_SYSTEM_EVENT_H_

#include <string>

#include "ext/json.hpp"
#include "include/lux_types.h"
#include "include/lux_utils.h"

namespace luxbracer {
class SystemEvent {
 private:
  std::string name;
 public:
  friend void from_json(const nlohmann::json& j, \
                    SystemEvent & se) {  // NOLINT(runtime/references)
    j.at("name").get_to(se.name);
  }
};
}  // namespace luxbracer

#endif  // INCLUDE_SYSTEM_EVENT_H_
