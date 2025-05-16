/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef PLANET__H
#define PLANET__H

#include <cstdint>
#include <string>

#include "json.hpp"
#include "lux_types.h"

//#include "stateEntity,h"
//#include "accessPoint"

namespace luxbracer {

    class Planet {
        private:
            std::string     _name;
            std::string     _system;
            uint64_t        _id;

            planetGovernment government;
            int prosperity;
 //           List<stateEntity> _states
 //           List<accessPoint> _accessPoints
 //           planetType _type;
//            planetSize _size;
//            planetGravity _gravity;
//            planetTemp _temperature;
//            planetAtmoPress _atmospherePressure;
//            bool _atmoBreathable;
//            biosphereType _biosphere;
        public:
            Planet(void) = default;
            Planet(const std::string& name, const std::string& system, uint64_t id);

            ~Planet(void);
            std::string name(void) const;
            std::string system(void) const;

            void randomize(void);

            friend void to_json(nlohmann::json& j, const Planet & p);
            friend void from_json(const nlohmann::json& j, Planet & p);
    };
}

#endif /* PLANET__H */
