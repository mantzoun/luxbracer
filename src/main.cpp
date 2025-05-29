/*
 * Copyright 2025 Stavros Mantzouneas
 *
 * main.cpp
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <cstring>

#include "discord_postman.h"
#include "discord_bot.h"
#include "engine.h"

namespace luxbracer {
    static DiscordBot bot;
    Engine engine;

    Logger logger = Logger(LUX_LOG_DEBUG);

    // void game_loop() {
    //     logger.info("here");
    //     bot.post_message("syslog", "here");
    // }

    int main(int argc, char** argv) {
        logger.info("Starting Discord Bot\n");

        engine.setLogger(&logger);
        engine.set_postman(&bot);
#ifdef ENGINE_INIT_FROM_DISCORD
        // prepare the engine to receive data from the bot
        engine.init();
#endif /* ENGINE_INIT_FROM_DISCORD */

        bot.set_logger(&logger);
        bot.set_engine(&engine);
        std::string token = "";
        std::string id = "myid";
        bot.init(token, id);

        while (!bot.init_complete) {
            logger.debug("Waiting for bot init");
            usleep(5 * 1000 * 1000);
        }

#ifndef ENGINE_INIT_FROM_DISCORD
    // inialize the engine and configure the discord channels
    engine.init();
    engine.create_channels();
#endif /* ENGINE_INIT_FROM_DISCORD */

        while (1) {
            // game_loop();
            engine.execute_game_loop();
            usleep(60 * 1000 * 1000);
        }
    }
}  // namespace luxbracer

int main(int argc, char** argv) {
    return luxbracer::main(argc, argv);
}
