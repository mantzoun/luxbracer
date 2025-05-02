#include "engine.h"

namespace luxbracer {
    Engine::Engine(){

    }

    uint64_t Engine::next_id() {
        return this->index++;
    }

    engineError Engine::systemAdd(System system) {
        if ( _systems.count(system.name()) != 0 ) {
            logger->warn("System " + system.name() + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        logger->debug("System " + system.name() + "(" + std::to_string(system.id()) + ") added");
        _systems.insert({system.name(), system});
        return ENGINE_OK;
    }

     engineError Engine::systemAdd(std::string name) {
        if (_systems.count(name ) != 0) {
            logger->warn("System " + name + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        System s = System(name, this->next_id());

        return this->systemAdd(s);
    }

    void Engine::setLogger(Logger * l) {
        logger = l;
    }
}