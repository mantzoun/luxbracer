#include "system.h"

namespace luxbracer {
    System::System(const std::string& name, uint64_t id)
        : _name(name),
          _id(id)
    {
    }

    systemError System::planetAdd(Planet planet) {
        if ( _planets.count(planet.name()) != 0 ) {
            logger->warn("Planet " + planet.name() + " exists");
            return SYSTEM_ITEM_EXISTS;
        }

        _planets.insert({planet.name(), planet});
        return SYSTEM_OK;
    }

    std::string System::name() const {
        return _name;
    }

    uint64_t System::id() {
        return this->_id;
    }

    void System::setLogger(Logger * l) {
        logger = l;
    }
}
