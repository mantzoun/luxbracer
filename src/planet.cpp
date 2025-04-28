#include "planet.h"

namespace luxbracer {
    Planet::Planet(const std::string& name, const std::string& system, uint64_t id) 
        : _name(name),
          _system(system),
          _id(id)
    {
    }

    std::string Planet::name() const {
        return _name;
    }
}
