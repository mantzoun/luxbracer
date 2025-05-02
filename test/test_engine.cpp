#include "catch.hpp"
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
}
