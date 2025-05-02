#ifndef ENGINE__H
#define ENGINE__H

#include <map>

#include "system.h"

namespace luxbracer {
    enum engineError {
        ENGINE_OK,
        ENGINE_ERROR,
        ENGINE_ITEM_EXISTS,
    };

    class Engine {
        private:
            uint64_t index = 0;

            std::map<std::string, System> _systems;

            Logger * logger = NULL;

        public:
            Engine(void);

            uint64_t next_id(void);

            engineError systemAdd(System system);
            engineError systemAdd(std::string name);
            void setLogger(Logger * l);
    };
}

#endif /* ENGINE__H */