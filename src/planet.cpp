/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "planet.h"
#include "lux_utils.h"

namespace luxbracer {
    Planet::Planet(const std::string& name, const std::string& system, uint64_t id)
        : _name(name),
          _system(system),
          _id(id) {
        this->randomize();
    }

    Planet::~Planet(void) {
    }

    std::string Planet::name() const {
        return _name;
    }

    std::string Planet::system() const {
        return _system;
    }

    void Planet::randomize() {
        LuxUtils utils;
        this->government = static_cast<planetGovernment>(utils.randomInt(0, PLANET_GOV_MAX));
        this->prosperity = utils.randomInt(10,200);
    }

    void to_json(nlohmann::json& j, const Planet & p) {  // NOLINT(runtime/references)
         j = nlohmann::json{
             {"name", p._name},
             {"id", p._id},
             {"system", p._system},
             {"government", p.government},
             {"prosperity", p.prosperity},
         };
     }

      void from_json(const nlohmann::json& j, Planet & p) {  // NOLINT(runtime/references)
         j.at("name").get_to(p._name);
         j.at("id").get_to(p._id);
         j.at("system").get_to(p._system);
         j.at("government").get_to(p.government);
         j.at("prosperity").get_to(p.prosperity);
     }
}  // namespace luxbracer
