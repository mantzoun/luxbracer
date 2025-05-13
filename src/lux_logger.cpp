/*
 * Copyright 2025 Stavros Mantzouneas
 *
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
    std::string lux_log_lvl_str[luxbracer::LUX_LOG_ERROR + 1] = {
           "DEBUG  ",
           "INFO   ",
           "WARNING",
           "ERROR  "
        };

    Logger::Logger(log_lvl lvl) {
        this->set_level(lvl);
    }

    void Logger::set_level(log_lvl lvl) {
        this->level = lvl;
    }

    void Logger::log(log_lvl lvl, const std::string& msg) {
        if (lvl >= this->level) {
            std::scoped_lock lock(mutex);

            time_t now = time(0);
            tm ltm;
            localtime_r(&now, &ltm);
            char res[40];
            snprintf(res, sizeof(res), "%04d-%02d-%02d %02d:%02d:%02d %s", 1900 + ltm.tm_year,
                                                                1 + ltm.tm_mon, ltm.tm_mday,
                                                                ltm.tm_hour, ltm.tm_min, ltm.tm_sec,
                                                                lux_log_lvl_str[lvl].c_str());
            std::string tstamp = res;
            std::cout << tstamp + " " + msg + "\n";
            std::cout.flush();
        }
    }

    void Logger::debug(const std::string& msg) {
        this->log(LUX_LOG_DEBUG, msg);
    }

    void Logger::info(const std::string& msg) {
        this->log(LUX_LOG_INFO, msg);
    }

    void Logger::warn(const std::string& msg) {
        this->log(LUX_LOG_WARN, msg);
    }

    void Logger::error(const std::string& msg) {
        this->log(LUX_LOG_ERROR, msg);
    }
}  // namespace luxbracer
