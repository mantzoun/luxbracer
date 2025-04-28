#include "discord_channel.h"

namespace luxbracer {
    DiscordChannel::DiscordChannel(dpp::snowflake id, dpp::snowflake parent, std::string name) {
        _name = name;
        _id = id;
        _parent = parent;
    }

    dpp::snowflake DiscordChannel::id(void) const {
        return _id;
    }

    dpp::snowflake DiscordChannel::parent(void) const{
        return _parent;
    }

    std::string DiscordChannel::name(void) const{
        return _name;
    }

    void DiscordChannel::set_parent(dpp::snowflake parent) {
        _parent = parent;
    }

    void DiscordChannel::set_name(const std::string& name) {
        _name = name;
    }
}