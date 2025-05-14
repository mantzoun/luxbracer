/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef DISCORD_POSTMAN_H
#define DISCORD_POSTMAN_H

namespace luxbracer {
    class DiscordPostman {
        public:
            virtual ~DiscordPostman() = default;

            virtual void post(const std::string & message, const std::string & channel) = 0;
            virtual void add_system_channel(const std::string & system_name) = 0;
            virtual void add_planet_channel(const std::string & planet_name,
                                            const std::string & system_name) = 0;
    };
}

#endif /* DISCORD_POSTMAN_H */
