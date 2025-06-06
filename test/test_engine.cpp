#include "ext/catch.hpp"
#include "ext/json.hpp"
#include "include/engine.h"

namespace luxbracer {
    struct EngineTestcase {
        EngineTestcase()
            : logger(LUX_LOG_DEBUG)
        {
            engine.setLogger(&logger);
        }

        ~EngineTestcase() {
        }

        Engine engine;
        Logger logger;
    };

    TEST_CASE_METHOD(EngineTestcase, "CheckAddSystemsByName", "ENGINE_GROUP") {
        REQUIRE(engine.systemAdd("System1") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System2") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System3") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System4") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System5") == ENGINE_OK);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddSystemDuplicatesByName", "ENGINE_GROUP") {
        REQUIRE(engine.systemAdd("System1") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System2") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System1") == ENGINE_ITEM_EXISTS);
        REQUIRE(engine.systemAdd("System4") == ENGINE_OK);
        REQUIRE(engine.systemAdd("System2") == ENGINE_ITEM_EXISTS);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddSystemsByObject", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System2", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System3", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System4", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System5", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddSystemDuplicatesByObject", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System2", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System1", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_ITEM_EXISTS);

        s = System("System4", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System2", engine.next_id());
        REQUIRE(engine.systemAdd(s) == ENGINE_ITEM_EXISTS);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddPlanetOK", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("planet1", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("planet2", "System1") == ENGINE_OK);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddPlanetDuplicate", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("planet1", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("planet1", "System1") == ENGINE_ITEM_EXISTS);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckRemovePlanetOK", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("planet1", "System1") == ENGINE_OK);
        Planet * p  = engine.planetGet("planet1", "System1");
        REQUIRE(p->getName() == "planet1");
        REQUIRE(p->getSystem() == "System1");

        REQUIRE(engine.planetAdd("planet2", "System1") == ENGINE_OK);
        p = engine.planetGet("planet2", "System1");
        REQUIRE(p->getName() == "planet2");
        REQUIRE(p->getSystem() == "System1");

        REQUIRE(engine.planetRemove("planet1", "System2") == ENGINE_ERROR);

        REQUIRE(engine.planetRemove("planet2", "System1") == ENGINE_OK);
        REQUIRE(engine.planetRemove("planet1", "System1") == ENGINE_OK);

        REQUIRE(engine.planetRemove("planet1", "System1") == ENGINE_ERROR);
        REQUIRE(engine.planetGet("planet1", "System1") == NULL);

        REQUIRE(engine.planetRemove("planet2", "System1") == ENGINE_ERROR);
        REQUIRE(engine.planetGet("planet2", "System1") == NULL);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddPlanetWrongSystem", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("planet1", "System2") == ENGINE_ERROR);
        REQUIRE(engine.planetAdd("planet1", "System1") == ENGINE_OK);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckEngineSaveLoad", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System2", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("planet1", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("planet2", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("planet3", "System2") == ENGINE_OK);

        nlohmann::json j;
        to_json(j, engine);
        save_file("savefile.json", engine);

        Engine new_engine;

        from_json(load_file("savefile.json"), new_engine);

        System * s1 = new_engine.systemGet("System1");
        System * s2 = new_engine.systemGet("System2");
        System * s3 = new_engine.systemGet("System3");

        Planet * p1  = new_engine.planetGet("planet1", "System1");
        Planet * p2  = new_engine.planetGet("planet2", "System1");
        Planet * p30 = new_engine.planetGet("planet3", "System1");
        Planet * p31 = new_engine.planetGet("planet3", "System2");
        Planet * p32 = new_engine.planetGet("planet3", "System3");

        REQUIRE(s1 != NULL);
        REQUIRE(s1->getName() == "System1");

        REQUIRE(s2 != NULL);
        REQUIRE(s2->getName() == "System2");

        REQUIRE(s3 == NULL);

        REQUIRE(p1 != NULL);
        REQUIRE(p1->getName() == "planet1");
        REQUIRE(p1->getSystem() == "System1");

        REQUIRE(p2 != NULL);
        REQUIRE(p2->getName() == "planet2");
        REQUIRE(p2->getSystem() == "System1");

        REQUIRE(p30 == NULL);

        REQUIRE(p31 != NULL);
        REQUIRE(p31->getName() == "planet3");
        REQUIRE(p31->getSystem() == "System2");

        REQUIRE(p32 == NULL);
    }
}
