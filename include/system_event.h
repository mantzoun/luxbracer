/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef SYSTEM_EVENT_H
#define SYSTEM_EVENT_H

#include "json.hpp"
#include "lux_types.h"
#include "lux_utils.h"

namespace luxbracer {
    class SystemEvent {
        private:
            std::string name;
        public:
            friend void from_json(const nlohmann::json& j, SystemEvent & se) {
                  j.at("name").get_to(se.name);
              }
    };
}

#endif /* SYSTEM_EVENT_H */
