#include "catch.hpp"
#include "json.hpp"
#include "engine.h"

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

        REQUIRE(engine.planetAdd("Planet1", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("Planet2", "System1") == ENGINE_OK);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddPlanetDuplicate", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("Planet1", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("Planet1", "System1") == ENGINE_ITEM_EXISTS);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckRemovePlanetOK", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("Planet1", "System1") == ENGINE_OK);
        Planet * p  = engine.planetGet("Planet1", "System1");
        REQUIRE(p->name() == "Planet1");
        REQUIRE(p->system() == "System1");

        REQUIRE(engine.planetAdd("Planet2", "System1") == ENGINE_OK);
        p = engine.planetGet("Planet2", "System1");
        REQUIRE(p->name() == "Planet2");
        REQUIRE(p->system() == "System1");

        REQUIRE(engine.planetRemove("Planet1", "System2") == ENGINE_ERROR);

        REQUIRE(engine.planetRemove("Planet2", "System1") == ENGINE_OK);
        REQUIRE(engine.planetRemove("Planet1", "System1") == ENGINE_OK);

        REQUIRE(engine.planetRemove("Planet1", "System1") == ENGINE_ERROR);
        REQUIRE(engine.planetGet("Planet1", "System1") == NULL);

        REQUIRE(engine.planetRemove("Planet2", "System1") == ENGINE_ERROR);
        REQUIRE(engine.planetGet("Planet2", "System1") == NULL);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckAddPlanetWrongSystem", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("Planet1", "System2") == ENGINE_ERROR);
        REQUIRE(engine.planetAdd("Planet1", "System1") == ENGINE_OK);
    }

    TEST_CASE_METHOD(EngineTestcase, "CheckEngineSaveLoad", "ENGINE_GROUP") {
        System s = System("System1", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        s = System("System2", engine.next_id());
        s.setLogger(&logger);
        REQUIRE(engine.systemAdd(s) == ENGINE_OK);

        REQUIRE(engine.planetAdd("Planet1", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("Planet2", "System1") == ENGINE_OK);
        REQUIRE(engine.planetAdd("Planet3", "System2") == ENGINE_OK);

        nlohmann::json j;
        to_json(j, engine);
        engine.save("savefile.json", engine);

        Engine new_engine;

        from_json(new_engine.load("savefile.json"), new_engine);

        System * s1 = new_engine.systemGet("System1");
        System * s2 = new_engine.systemGet("System2");
        System * s3 = new_engine.systemGet("System3");

        Planet * p1  = new_engine.planetGet("Planet1", "System1");
        Planet * p2  = new_engine.planetGet("Planet2", "System1");
        Planet * p30 = new_engine.planetGet("Planet3", "System1");
        Planet * p31 = new_engine.planetGet("Planet3", "System2");
        Planet * p32 = new_engine.planetGet("Planet3", "System3");

        REQUIRE(s1 != NULL);
        REQUIRE(s1->name() == "System1");

        REQUIRE(s2 != NULL);
        REQUIRE(s2->name() == "System2");

        REQUIRE(s3 == NULL);

        REQUIRE(p1 != NULL);
        REQUIRE(p1->name() == "Planet1");
        REQUIRE(p1->system() == "System1");

        REQUIRE(p2 != NULL);
        REQUIRE(p2->name() == "Planet2");
        REQUIRE(p2->system() == "System1");

        REQUIRE(p30 == NULL);

        REQUIRE(p31 != NULL);
        REQUIRE(p31->name() == "Planet3");
        REQUIRE(p31->system() == "System2");

        REQUIRE(p32 == NULL);
    }
}
