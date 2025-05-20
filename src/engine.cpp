/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include <stdexcept>
#include "lux_utils.h"
#include "engine.h"

namespace luxbracer {
    Engine::Engine() {
    }

    Engine::Engine(Logger * l) : logger(l) {
    }

    void Engine::set_postman(DiscordPostman * p) {
        postman = p;
    }

    uint64_t Engine::next_id() {
        return this->index++;
    }

    void Engine::init() {
        // try to read initial conditions
        try {
            nlohmann::json json = load_file("initial.json");
            from_json(json, *this);
        } catch (const std::runtime_error & e) {
            this->logger->warn("Could not read initial conditions");
        }
    }

    void Engine::create_channels() {
        for (auto it = system_map.begin(); it != system_map.end(); ++it) {
            std::string system_name = it->first;
            System system = it->second;

            postman->add_system_channel(system_name);
            std::vector<std::string> planets = system.get_planet_names();

            for (const std::string & jt : planets) {
                postman->add_planet_channel(jt, system_name);
            }
        }
    }

//    void Engine::execute_player_actions() {
//    }

    void Engine::date_advance() {
        this->date++;
    }

//    void Engine::systems_advance() {
//    }
//
//    void Engine::planets_advanve() {
//    }

    engineError Engine::systemAdd(System & system) {
        if ( system_map.count(system.getName()) != 0 ) {
            logger->warn("System " + system.getName() + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        logger->debug("System " + system.getName() + "(" + std::to_string(system.getId()) + ") added");
        system_map.insert({system.getName(), system});
        return ENGINE_OK;
    }

     engineError Engine::systemAdd(const std::string& name) {
        if (system_map.count(name) != 0) {
            logger->warn("System " + name + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        System s = System(name, this->next_id());

        return this->systemAdd(s);
    }

    engineError Engine::planetAdd(const std::string & name, const std::string & system) {
        System * s = systemGet(system);

        if (s == NULL) {
            return ENGINE_ERROR;
        }

        return s->planetAdd(Planet(name, system, this->next_id()));
    }

    System * Engine::systemGet(const std::string & name) {
        try {
            System & system = system_map.at(name);
            return &system;
        }  catch (const std::out_of_range & e) {
            return NULL;
        }
    }

    Planet * Engine::planetGet(const std::string & name, const std::string & system) {
        System * s = systemGet(system);

        if (s == NULL) {
            return NULL;
        }

        return s->planetGet(name);
    }

    engineError Engine::planetRemove(const std::string & name, const std::string & system) {
        System * s = systemGet(system);

        if (s == NULL) {
            return ENGINE_ERROR;
        }

        return s->planetRemove(name);
    }

    void Engine::setLogger(Logger * l) {
        logger = l;
    }

    void Engine::execute_game_loop() {
        postman->post("Game Loop Test", "syslog");
//        this->execute_player_actions();
        this->date_advance();
//        this->systems_advance();
//        this->planets_advanve();

        nlohmann::json json;
        to_json(json, *this);
        save_file("engine_save_" + std::to_string(this->date) + ".json", json);
    }
}  // namespace luxbracer
