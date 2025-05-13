/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef DISCORD_CHANNEL__H
#define DISCORD_CHANNEL__H

#include "dpp/dpp.h"


namespace luxbracer {
    class DiscordChannel {
        private:
            dpp::snowflake  _id;
            dpp::snowflake  _parent;
            std::string _name;
        public:
            DiscordChannel(dpp::snowflake id, dpp::snowflake parent, const std::string & name);

            dpp::snowflake  id(void) const;
            dpp::snowflake  parent(void) const;
            std::string     name(void) const;

            void set_parent(dpp::snowflake parent);
            void set_name(const std::string& name);

            bool operator==(const DiscordChannel& other) const {
                return (_id == other.id() &&
                        _parent == other.parent() &&
                        _name == other.name());
            }
    };
}

#endif /* DISCORD_CHANNEL__H */
