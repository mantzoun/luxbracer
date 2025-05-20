/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef LUX_UTILS__H
#define LUX_UTILS__H

#include <random>
#include <stdexcept>
#include <fstream>

#include "json.hpp"

namespace luxbracer {
    inline
    void save_file(const std::string & filename, const nlohmann::json & j) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
        file << j.dump(4);
    }

    inline
    nlohmann::json load_file(const std::string & filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }
        nlohmann::json j;
        file >> j;
        return j;
    }


    class LuxUtils {
        private:
                static std::mt19937& getEngine() {
                static std::random_device rd;
                static std::mt19937 engine(rd());
                return engine;
            }
        public:
            LuxUtils(void) {
                srand( (unsigned)time(NULL) );
            }

            static int randomInt(int min, int max) {
                std::uniform_int_distribution<int> dist(min, max);
                return dist(getEngine());
            }
    };
}

#endif /* LUX_UTILS__H */