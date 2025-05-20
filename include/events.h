/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef EVENTS_H
#define EVENTS_H

#include "json.hpp"
#include "lux_types.h"
#include "lux_utils.h"
#include "planet_event.h"
#include "system_event.h"
#include "planet.h"
#include "system.h"

namespace luxbracer {
    class Events {
        private:
            Logger * logger; // no_serial

            PlanetEvents    planet_events;
            SystemEvents    system_events;
        public:
            explicit Events(Logger * logger);

            void apply_events(Planet * planet);
            void process_events(Planet * planet);

            void apply_events(System * system);
            void process_events(System * system);

            friend void from_json(const nlohmann::json& j, Events & e);
    };

    inline
    void from_json(const nlohmann::json& j, Events & e) {  // NOLINT(runtime/references)
        j.at("planet_events").get_to(e.planet_events);
        j.at("system_events").get_to(e.system_events);
    }
}

#endif /* EVENTS_H */
