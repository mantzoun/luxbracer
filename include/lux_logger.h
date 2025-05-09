/*
 * lux_logger.h
 *
 * Implement all logger functionality in one object, shared
 * between threads. need to check thread safety
 */

#ifndef LUX_LOGGER__H
#define LUX_LOGGER__H

#include <string>

namespace luxbracer {
    /**
     * @brief Supported logger log severity levels
     */
    enum log_lvl
    {
        LUX_LOG_DEBUG,
        LUX_LOG_INFO,
        LUX_LOG_WARN,
        LUX_LOG_ERROR
    };

    /**
     * @class Logger
     *
     * @brief The logger object will handle logging for all
     *        application modules.
     */
    class Logger
    {
        private:
            log_lvl level;

        public:
            /**
             * @brief Custom logger constructor
             * @param lvl The log severity level
             */
            explicit Logger(log_lvl lvl);

            /**
             * @brief Set the log level
             *
             * @param lvl The log level
             */
            void set_level(log_lvl lvl);

            /**
             * @brief Print a message to stdout, if it exceeds the configured severity
             *
             * @param lvl The message severity
             * @param msg The log message
             */
            void    log(log_lvl lvl, const std::string& msg);

            /**
             * @brief wrapper that calls the log() function with DEBUG severity
             *
             * @param msg The log message
             */
            void    debug(const std::string& msg);

            /**
             * @brief wrapper that calls the log() function with INFO severity
             *
             * @param msg The log message
             */
            void    info(const std::string& msg);

            /**
             * @brief wrapper that calls the log() function with WANR severity
             *
             * @param msg The log message
             */
            void    warn(const std::string& msg);

            /**
             * @brief wrapper that calls the log() function with ERRROR severity
             *
             * @param msg The log message
             */
            void    error(const std::string& msg);
    };
}

#endif /* LUX_LOGGER__H */
