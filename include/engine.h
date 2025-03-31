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
            std::map<std::string, System> _systems;

            Logger * logger = NULL;

        public:
            Engine(void);

            engineError systemAdd(System s);
            void setLogger(Logger * l);
    };
}

#endif /* ENGINE__H */