#include "discord_guild.h"


static std::string  str2lower(std::string in_string) {
    std::transform(in_string.begin(), in_string.end(), in_string.begin(), [](unsigned char c) { return std::tolower(c); });

    return in_string;
}

namespace luxbracer {
    DiscordGuild::DiscordGuild(uint64_t  id){
        _id = id;
    }

    uint64_t  DiscordGuild::id(void) {
        return _id;
    }

    void DiscordGuild::channel_add(DiscordChannel * channel) {
        channels.push_back(channel);
    }

    std::list<DiscordChannel *> DiscordGuild::channel_get(std::string name, uint64_t channel_id, uint64_t parent_id) {
        std::list<DiscordChannel *> result;

        for (DiscordChannel * channel : channels) {
            if (str2lower(channel->name()) == str2lower(name) &&
                        (parent_id == 0 || channel->parent() == parent_id) &&
                        (channel_id == 0 || channel->id() == channel_id)) {
                result.push_back(channel);
            }
        }

        return result;  
    }

    void DiscordGuild::channel_delete(uint64_t  channel_id, std::string name, uint64_t  parent_id) {

    }
}