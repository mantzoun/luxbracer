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
    Logger logger = Logger(LUX_LOG_DEBUG);

    void game_loop() {
        logger.info("here");
        bot.post_message("syslog", "here");
    }

    int main(int argc, char** argv)
    {
        logger.info("Starting Discord Bot\n");

        engine.setLogger(&logger);

        bot.set_logger(&logger);
        bot.set_engine(&engine);
        std::string token = "MTMwNTUwNzA4ODczMDE2MTIwMg.Gymvpe.GTUp2WktgzBpmxwnrhRrticWyNysvbhDwM0HJg";
        std::string id = "myid";
        bot.init(token, id);

        while(1){
            usleep(60 * 1000 * 1000);
            game_loop();
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
