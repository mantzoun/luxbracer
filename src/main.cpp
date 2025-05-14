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

        Planet p("p1", "s1", 0);
        System s("s1", 0);

//    cdb::Logger logger = cdb::Logger(cdb::CDB_LOG_DEBUG);

//    cdb::IO io;
//    cdb::Configurator conf;
//    cdb::MqttHandler m_handler;

//    conf.set_logger(&logger);
//    conf.config_read(config_file);
//    logger.set_level(conf.get_log_level());

//    m_handler.set_logger(&logger);
//    io.set_logger(&logger);

//    logger.info("Reading config file");

//    io.fifo_init("/tmp/mqtt_disc_cpp.fifo");

//    logger.info("Starting Discord Bot\n");
//    bot.set_logger(&logger);
    std::string token = "";
    std::string id = "myid";
    bot.init(token, id);

//    bot.set_mqtt_handler(&m_handler);

//    m_handler.set_discord_bot(&bot);
//    m_handler.init(&conf);

//    io.set_discord_bot(&bot);

//    logger.info("Init done, entering loop");

    while(1){
        usleep(1000000);
        engine.setLogger(&logger);
        engine.set_postman(&bot);
#ifdef ENGINE_INIT_FROM_DISCORD
        // prepare the engine to receive data from the bot
        engine.init();
#endif /* ENGINE_INIT_FROM_DISCORD */

        bot.set_logger(&logger);
        bot.set_engine(&engine);
        std::string token = ""
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
