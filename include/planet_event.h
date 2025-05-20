/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef PLANET_EVENT_H
#define PLANET_EVENT_H

#include "json.hpp"
#include "lux_types.h"
#include "lux_utils.h"

namespace luxbracer {
    class PlanetEvent {
        private:
            std::string name;
        public:
            friend void from_json(const nlohmann::json& j, PlanetEvent & pe) {
                j.at("name").get_to(pe.name);
            }
    };
}

#endif /* PLANET_EVENT_H */
