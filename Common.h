#pragma once

#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <random>

#include "Ray.h"

// Constants

const float c_Infinity = std::numeric_limits<float>::infinity();
const float c_Pi = 3.1415926535897932385;

inline float degreeToRadians(float angle) { return angle * c_Pi / 180.0f; }

inline float clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline float ffmin(float a, float b) { return a <= b ? a : b; }
inline float ffmax(float a, float b) { return a >= b ? a : b; }

inline double randomDouble() {
    return rand() / (RAND_MAX + 1.0);
}

// Returns a random real number in [min,max)
inline double randomDouble(double min, double max)
{
    return min + (max-min) * randomDouble();
}

inline double randomDoubleCpp()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

inline int randomInt(int min, int max)
{
    return static_cast<int>(randomDouble(min, max+1));
}

inline glm::vec3 randomVec3()
{
    return glm::vec3(randomDouble() * randomDouble(),
                     randomDouble() * randomDouble(),
                     randomDouble() * randomDouble());
}

inline glm::vec3 randomVec3(double min, double max)
{
    return glm::vec3(randomDouble(min, max) * randomDouble(min, max),
                     randomDouble(min, max) * randomDouble(min, max),
                     randomDouble(min, max) * randomDouble(min, max));
}

