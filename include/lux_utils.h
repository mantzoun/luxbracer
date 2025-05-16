/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef ENGINE_UTILS__H
#define ENGINE_UTILS__H

#include <random>

namespace luxbracer {
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

#endif /* ENGINE_UTILS__H */