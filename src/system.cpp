/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include <stdexcept>

#include "system.h"

namespace luxbracer {
    System::System(const std::string& name, uint64_t id) :
        name(name),
        id(id) {
    }

    engineError System::planetAdd(Planet planet) {
        Planet * p = planetGet(planet.getName());

        if (p != NULL) {
            logger->warn("Planet " + planet.getName() + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        planet_map.insert({planet.getName(), planet});
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

    std::vector<std::string> System::get_planet_names(void) {
        std::vector<std::string> planets;

        for (auto it = this->planet_map.begin(); it != this->planet_map.end(); ++it) {
            planets.push_back(it->first);
        }

        return planets;
    }

    std::string System::getName() const {
        return this->name;
    }

    uint64_t System::getId() const {
        return this->id;
    }

    void System::setLogger(Logger * l) {
        logger = l;
    }

}  // namespace luxbracer
