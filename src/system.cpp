#include <stdexcept>

#include "system.h"

namespace luxbracer {
    System::System(const std::string& name, uint64_t id) :
        _name(name),
        _id(id)
    {
    }

    engineError System::planetAdd(Planet planet) {
        Planet * p = planetGet(planet.name());

        if (p != NULL) {
            logger->warn("Planet " + planet.name() + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        planet_map.insert({planet.name(), planet});
        return ENGINE_OK;
    }

    Planet * System::planetGet(const std::string & name) {
        try {
            Planet & p = planet_map.at(name);
            return &p;
        } catch (const std::out_of_range& e) {
            return NULL;
        }
    }

    engineError System::planetRemove(const std::string & name) {
        Planet * p = planetGet(name);

        if (p == NULL) {
            logger->warn("Planet " + name + " not in list");
            return ENGINE_ERROR;
        }

        planet_map.erase(name);

        return ENGINE_OK;
    }

    std::string System::name() const {
        return _name;
    }

    uint64_t System::id() const {
        return this->_id;
    }

    void System::setLogger(Logger * l) {
        logger = l;
    }

    void to_json(nlohmann::json& j, const System & s) {
        j = nlohmann::json{
            {"name", s._name},
            {"id", s._id},
            {"planet_map", s.planet_map}
        };
    }

     void from_json(const nlohmann::json& j, System & s) {
        j.at("name").get_to(s._name);
        j.at("id").get_to(s._id);
        j.at("planet_map").get_to(s.planet_map);
    }
}
