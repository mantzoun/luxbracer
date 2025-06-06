/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "include/planet.h"
#include "include/lux_utils.h"

namespace luxbracer {
Planet::Planet(const std::string& name,
               const std::string& system,
               uint64_t id) : name(name), system(system), id(id) {
  this->randomize();
}

Planet::~Planet(void) {
}

std::string Planet::getName() const {
  return name;
}

std::string Planet::getSystem() const {
  return system;
}

void Planet::randomize() {
  LuxUtils utils;
  this->government = static_cast<planetGovernment>(
                                    utils.randomInt(0, PLANET_GOV_MAX));
  this->prosperity = utils.randomInt(10, 200);
}
}  // namespace luxbracer
