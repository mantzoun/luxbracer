/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef SYSTEM__H
#define SYSTEM__H

#include <cstdint>
#include <string>
#include <map>

#include "json.hpp"
//#include "stateEntity,h"
//#include "accessPoint"
#include "engine_types.h"
#include "planet.h"
#include "lux_logger.h"

namespace luxbracer {
    class System {
        private:
            Logger * logger = NULL;
            std:: string    _name;
            uint64_t        _id;

            std::map<std::string, Planet> planet_map;
        public:
            System(void) = default;
            System(const std::string& name, uint64_t id);

            std::string name(void) const;

            uint64_t id(void) const;

            engineError planetAdd(Planet planet);
            engineError planetRemove(const std::string & name);
            Planet * planetGet(const std::string & name);

            std::vector<std::string> get_planet_names(void);

            void setLogger(Logger * l);

            friend void to_json(nlohmann::json& j, const System & s);
            friend void from_json(const nlohmann::json& j, System & s);
    };
}

#endif /* SYSTEM__H */
