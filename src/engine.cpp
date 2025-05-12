#include <stdexcept>
#include "engine.h"

namespace luxbracer {
    Engine::Engine(){

    }

    void Engine::set_postman(DiscordPostman * p) {
        postman = p;
    }

    uint64_t Engine::next_id() {
        return this->index++;
    }

    void Engine::execute_player_actions() {

    }

    void Engine::date_advance() {

    }

    void Engine::systems_advance() {

    }

    void Engine::planets_advanve() {

    }

    engineError Engine::systemAdd(System & system) {
        if ( system_map.count(system.name()) != 0 ) {
            logger->warn("System " + system.name() + " exists");
            return ENGINE_ITEM_EXISTS;
        }

        logger->debug("System " + system.name() + "(" + std::to_string(system.id()) + ") added");
        system_map.insert({system.name(), system});
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
        this->execute_player_actions();
        this->date_advance();
        this->systems_advance();
        this->planets_advanve();
    }

    void to_json(nlohmann::json& j, const Engine & e) {
        j = nlohmann::json{
            {"index", e.index},
            {"system_map", e.system_map}
        };
    }

    void from_json(const nlohmann::json& j, Engine & e) {
        j.at("index").get_to(e.index);
        j.at("system_map").get_to(e.system_map);
    }

    void Engine::save(const std::string & filename, const nlohmann::json & j) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
        file << j.dump(4);
    }

    nlohmann::json Engine::load(const std::string & filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }
        nlohmann::json j;
        file >> j;
        return j;
    }
}
