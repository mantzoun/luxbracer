#include "system.h"

namespace luxbracer {
    System::System(std::string name) {
        _name = name;
    }

    systemError System::planetAdd(Planet planet) {
        if ( _planets.count(planet.name()) != 0 ) {
            logger->warn("Planet " + planet.name() + " exists");
            return SYSTEM_ITEM_EXISTS;
        }

        _planets.insert({planet.name(), planet});
        return SYSTEM_OK;
    }

    std::string System::name() {
        return _name;
    }

    void System::setLogger(Logger * l) {
        logger = l;
    }
    
}