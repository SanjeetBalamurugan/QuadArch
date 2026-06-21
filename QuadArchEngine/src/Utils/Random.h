#pragma once
#include "QuadArch/Core.h"

#include <iostream>
#include <random>

namespace QuadArch {
    class QuadAPI Random {
    public:
        Random() = delete;

        static int RandINT(int min, int max) {
            return std::uniform_int_distribution<int>{min, max}(get_engine());
        }
        static double RandDOUBLE(double min, double max) {
            return std::uniform_real_distribution<double>{min, max}(get_engine());
        }
        static bool RandBOOL(double probability = 0.5) {
            return std::bernoulli_distribution{ probability }(get_engine());
        }

    private:
        static std::mt19937& get_engine() {
            thread_local std::random_device rd;
            thread_local std::mt19937 engine(rd());
            return engine;
        }
    };
}
