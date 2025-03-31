#include "discord_channel.h"

namespace luxbracer {
    DiscordChannel::DiscordChannel(uint64_t  id, uint64_t  parent, std::string name) {
        _name = name;
        _id = id;
        _parent = parent;
    }

    uint64_t  DiscordChannel::id(void) {
        return _id;
    }

    uint64_t  DiscordChannel::parent(void) {
        return _parent;
    }

    std::string DiscordChannel::name(void) {
        return _name;
    }
}