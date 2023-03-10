#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(const double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(const double min, const double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline double clamp(const double x, const double min, const double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}