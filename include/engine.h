#ifndef ENGINE__H
#define ENGINE__H

#include <map>
#include <fstream>

#include "engine_types.h"
#include "system.h"
#include "json.hpp"

namespace luxbracer {
    class Engine {
        private:
            uint64_t index = 0;

            std::map<std::string, System> system_map;

            Logger * logger = NULL;

        public:
            Engine(void);

            uint64_t next_id(void);

            engineError systemAdd(System & system);
            engineError systemAdd(const std::string & name);
            engineError planetAdd(const std::string & name, const std::string & system);

            void setLogger(Logger * l);

            System * systemGet(const std::string & name);
            Planet * planetGet(const std::string & name, const std::string & system);
            engineError planetRemove(const std::string & name, const std::string & system);

            static void save(const std::string & filename, const nlohmann::json & j);
            static nlohmann::json load(const std::string & filename);

            friend void to_json(nlohmann::json& j, const Engine& e);
            friend void from_json(const nlohmann::json& j, Engine& e);
    };
}

#endif /* ENGINE__H */
