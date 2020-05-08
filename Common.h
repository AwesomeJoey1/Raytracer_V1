#pragma once

#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <random>

#include <glm/glm/gtx/norm.hpp>

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

// returns a random reflection referring to a Lambertian diffuse material
inline glm::vec3 randomUnitVector()
{
    auto a = randomDouble(0, 2*c_Pi);
    auto z = randomDouble(-1, 1);
    auto r = glm::sqrt(1-z*z);
    return glm::vec3(r*cos(a),r*sin(a),z);
}

inline glm::vec3 randomVec3()
{
    return glm::vec3(randomDouble() * randomDouble(),
                     randomDouble() * randomDouble(),
                     randomDouble() * randomDouble());
}

inline glm::vec3 randomVec3(double min, double max)
{
    return glm::vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline glm::vec3 randomInUnitSphere() {
    while (true) {
        auto p = randomVec3(-1,1);
        if (glm::length2(p) >= 1) continue;
        return p;
    }
}

inline glm::vec3 randomInHemisphere(const glm::vec3& normal) {
    glm::vec3 inUnitSphere = randomInUnitSphere();
    if (dot(inUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return inUnitSphere;
    else
        return -inUnitSphere;
}