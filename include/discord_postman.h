#ifndef DISCORD_POSTMAN_H
#define DISCORD_POSTMAN_H

namespace luxbracer {
    class DiscordPostman {
        public:
            virtual void post(const std::string & message, const std::string & channel) = 0;
            virtual ~DiscordPostman() = default;
    };
}

#endif /* DISCORD_POSTMAN_H */