/*
 * lux_logger.cpp
 *
 * implementation of Logger class methods
 */

#include <iostream>
#include <ctime>
#include <cstdio>
#include <mutex>

#include "lux_logger.h"

static std::mutex mutex; 

namespace luxbracer {
    std::string lux_log_lvl_str[luxbracer::LUX_LOG_ERROR + 1] =
                                 {
                                    "DEBUG  ",
                                    "INFO   ",
                                    "WARNING",
                                    "ERROR  "
                                 };

    Logger::Logger(log_lvl lvl)
    {
        this->set_level(lvl);
    }

    void Logger::set_level(log_lvl lvl)
    {
        this->level = lvl;
    }

    void Logger::log(log_lvl lvl, std::string s)
    {
        if (lvl >= this->level){
            std::scoped_lock lock(mutex);

            time_t now = time(0);
            tm *ltm = localtime(&now);
            char res[30];
            sprintf(res, "%02d-%02d-%02d %02d:%02d:%02d %s", ltm->tm_year, ltm->tm_mon, ltm->tm_mday,
                                                             ltm->tm_hour, ltm->tm_min, ltm->tm_sec,
                                                             lux_log_lvl_str[lvl].c_str());
            std::string tstamp = res;
            std::cout << tstamp + " " + s + "\n";
            std::cout.flush();
        }
    }

    void Logger::debug(std::string s)
    {
        this->log(LUX_LOG_DEBUG, s);
    }

    void Logger::info(std::string s)
    {
        this->log(LUX_LOG_INFO, s);
    }

    void Logger::warn(std::string s)
    {
        this->log(LUX_LOG_WARN, s);
    }

    void Logger::error(std::string s)
    {
        this->log(LUX_LOG_ERROR, s);
    }
}
