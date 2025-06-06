/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef INCLUDE_PLANET_H_
#define INCLUDE_PLANET_H_

#include <cstdint>
#include <string>

#include "ext/json.hpp"
#include "include/lux_types.h"

namespace luxbracer {
class Planet {
 private:
  std::string     name;
  std::string     system;
  uint64_t        id;

  planetGovernment government;
  int prosperity;
 public:
  Planet(void) = default;
  Planet(const std::string& name, const std::string& system, uint64_t id);

  ~Planet(void);
  std::string getName(void) const;
  std::string getSystem(void) const;

  void randomize(void);

  friend void to_json(nlohmann::json& j, \
                      const Planet & p);  // NOLINT(runtime/references)
  friend void from_json(const nlohmann::json& j, \
                        Planet & p);  // NOLINT(runtime/references)
};

inline
void to_json(nlohmann::json& j, \
             const Planet & p) {  // NOLINT(runtime/references)
  j = nlohmann::json{
      {"name", p.name},
      {"id", p.id},
      {"system", p.system},
      {"government", p.government},
      {"prosperity", p.prosperity},
  };
}

inline
void from_json(const nlohmann::json& j,
                Planet & p) {  // NOLINT(runtime/references)
  j.at("name").get_to(p.name);
  j.at("id").get_to(p.id);
  j.at("system").get_to(p.system);
  j.at("government").get_to(p.government);
  j.at("prosperity").get_to(p.prosperity);
}
}  // namespace luxbracer

#endif  // INCLUDE_PLANET_H_
