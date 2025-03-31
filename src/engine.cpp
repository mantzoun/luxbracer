#include "engine.h"

namespace luxbracer {
    Engine::Engine(){
    }

    engineError Engine::systemAdd(System system) {
        if ( _systems.count(system.name()) != 0 ) {
            logger->warn("System " + system.name() + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        _systems.insert({system.name(), system});
        return ENGINE_OK;
    }

    void Engine::setLogger(Logger * l) {
        logger = l;
    }
}