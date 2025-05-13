/*
 * Copyright 2025 Stavros Mantzouneas
 */
#include "discord_guild.h"

static std::string  str2lower(std::string in_string) {
    std::transform(in_string.begin(), in_string.end(), in_string.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

    return in_string;
}

namespace luxbracer {
    DiscordGuild::DiscordGuild(const dpp::snowflake id)
        : _id(id) {
    }

    void DiscordGuild::set_logger(Logger * logger) {
        this->logger = logger;
    }

    dpp::snowflake  DiscordGuild::id(void) const {
        return _id;
     }

    int DiscordGuild::verify_and_add_channel(const DiscordChannel& channel) {
        if (std::any_of(channels.begin(), channels.end(), [&](const auto& item) {
            return item == channel;
        })) {
            // channel exists
            logger->warn("Channel already exists : " + channel.name() +
                         "," + std::to_string(channel.id()) +
                         "," + std::to_string(channel.parent()));
            return -1;
        }

        channels.push_back(channel);
        logger->debug("Added channel : " + channel.name() +
                      "," + std::to_string(channel.id()) +
                      "," + std::to_string(channel.parent()));
        return 0;
    }

    int DiscordGuild::verify_and_delete_channel(const DiscordChannel& channel) {
        for (auto it = channels.begin(); it != channels.end(); ) {
            if (*it == channel) {
                channels.erase(it);
                logger->debug("Deleted channel : " + channel.name() +
                              "," + std::to_string(channel.id()) +
                              "," + std::to_string(channel.parent()));
                return 0;
            }
            it++;
        }

        logger->warn("Could not delete channel : " + channel.name() +
                     "," + std::to_string(channel.id()) +
                     "," + std::to_string(channel.parent()));
        return -1;
    }

    int DiscordGuild::verify_and_update_channel(const DiscordChannel& channel) {
        for (auto it = channels.begin(); it != channels.end(); ) {
            if (it->id() == channel.id()) {
                it->set_name(channel.name());
                it->set_parent(channel.parent());
                logger->debug("Updated channel : " + channel.name() +
                              "," + std::to_string(channel.id()) +
                              "," + std::to_string(channel.parent()));
                return 0;
            }
            it++;
        }

        logger->warn("Could not update channel : " + channel.name() +
                     "," + std::to_string(channel.id()) +
                     "," + std::to_string(channel.parent()));
        return -1;
    }

    int DiscordGuild::channel_add(const dpp::channel & channel) {
        return verify_and_add_channel(DiscordChannel(channel.id,
                                                     channel.parent_id, channel.name));
    }

    int DiscordGuild::channel_add(const dpp::channel_create_t & channel) {
        return verify_and_add_channel(DiscordChannel(channel.created.id,
                                                     channel.created.parent_id, channel.created.name));
    }

    int DiscordGuild::channel_update(const dpp::channel_update_t & channel) {
        return verify_and_update_channel(DiscordChannel(channel.updated.id,
                                                        channel.updated.parent_id, channel.updated.name));
    }

    int DiscordGuild::channel_delete(const dpp::channel_delete_t & channel) {
        return verify_and_delete_channel(DiscordChannel(channel.deleted.id,
                                                        channel.deleted.parent_id, channel.deleted.name));
    }

    std::list<DiscordChannel *> DiscordGuild::channel_get(const std::string& name,
                                                          dpp::snowflake channel_id, dpp::snowflake parent_id) {
        std::list<DiscordChannel *> result;

        for (DiscordChannel& channel : channels) {
            if (str2lower(channel.name()) == str2lower(name) &&
                        (parent_id == 0 || channel.parent() == parent_id) &&
                        (channel_id == 0 || channel.id() == channel_id)) {
                logger->debug("Add search result channel : " + channel.name() +
                              "," + std::to_string(channel.id()) +
                              "," + std::to_string(channel.parent()));
                result.push_back(&channel);
            }
        }

        return result;
    }

    std::list<DiscordChannel> DiscordGuild::channel_get_all(void) const {
        return channels;
    }

    DiscordChannel * DiscordGuild::channel_get_by_id(dpp::snowflake id) {
        if (id == 0) {
            return NULL;
        }

        auto it = std::find_if(channels.begin(), channels.end(),
        [&](const auto& channel) { return id == channel.id(); });

        if (it != channels.end()) {
            return &(*it);
        }

        return NULL;
    }
}  // namespace luxbracer
