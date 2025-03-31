#ifndef SYSTEM__H
#define SYSTEM__H

#include <cstdint>
#include <string>
#include <map>

//#include "stateEntity,h"
//#include "accessPoint"
#include "planet.h"
#include "lux_logger.h"

namespace luxbracer {
    enum systemError {
        SYSTEM_OK,
        SYSTEM_ERROR,
        SYSTEM_ITEM_EXISTS,
    };

    class System {
        private:
            Logger * logger = NULL;
            std:: string    _name;
            uint64_t        _id;

            std::map<std::string, Planet> _planets;
        public:
            System(std::string name);

            std::string name(void);

            systemError planetAdd(Planet planet);
            void setLogger(Logger * l);
    };
}

#endif /* SYSTEM__H */