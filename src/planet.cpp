#include "planet.h"

namespace luxbracer {
    Planet::Planet(std::string name, std::string system) {
        _name = name;
        _system = system;
    }  

    std::string Planet::name() {
        return _name;
    }
}