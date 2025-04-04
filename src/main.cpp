/*
 * main.cpp
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <cstring>

#include "discord_bot.h"
#include "engine.h"

namespace luxbracer {
    static DiscordBot bot;
    static Engine engine;

    int main(int argc, char** argv)
    {
        Logger logger = Logger(LUX_LOG_DEBUG);

        logger.info("Starting Discord Bot\n");
        bot.set_logger(&logger);
        std::string token = ""
        std::string id = "myid";
        bot.init(token, id);

        Planet p("p1", "s1");
        System s("s1");

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
        engine.systemAdd(s);
        s.planetAdd(p);
        
        while(1){
            logger.info("here");
            usleep(60 * 1000 * 1000);
        }
    }
}

int main(int argc, char** argv) {
    return luxbracer::main(argc, argv);
}




// #include <boost/archive/text_oarchive.hpp>
// #include <boost/archive/text_iarchive.hpp>
// #include <fstream>
// #include <iostream>

// class Person {
// private:
//     // Serialization needs access to private members
//     friend class boost::serialization::access;

//     // This function defines what to serialize/deserialize
//     template<class Archive>
//     void serialize(Archive &ar, const unsigned int version) {
//         ar & name;
//         ar & age;
//     }

//     std::string name;
//     int age;

// public:
//     // Default constructor needed for deserialization
//     Person() = default;
    
//     // Constructor for convenience
//     Person(const std::string& name, int age) : name(name), age(age) {}

//     // Function to display person details
//     void print() const {
//         std::cout << "Name: " << name << ", Age: " << age << '\n';
//     }
// };

// int main() {
//     // Serialize object to file
//     {
//         Person p1("John Doe", 30);
//         std::ofstream ofs("person.dat");
//         boost::archive::text_oarchive oa(ofs);
//         oa << p1;  // Serialize the object
//     }

//     // Deserialize object from file
//     {
//         Person p2;
//         std::ifstream ifs("person.dat");
//         boost::archive::text_iarchive ia(ifs);
//         ia >> p2;  // Deserialize into the object
//         p2.print();
//     }

//     return 0;
// }
