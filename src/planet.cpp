#include "planet.h"

namespace luxbracer {
    Planet::Planet(const std::string& name, const std::string& system, uint64_t id) 
        : _name(name),
          _system(system),
          _id(id)
    {
    }

    Planet::~Planet(void) {
    }

    std::string Planet::name() const {
        return _name;
    }

    std::string Planet::system() const {
        return _system;
    }

    void to_json(nlohmann::json& j, const Planet & p) {
         j = nlohmann::json{
             {"name", p._name},
             {"id", p._id},
             {"system", p._system}
         };
     }

      void from_json(const nlohmann::json& j, Planet & p) {
         j.at("name").get_to(p._name);
         j.at("id").get_to(p._id);
         j.at("system").get_to(p._system);
     }
}
