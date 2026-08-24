#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include "Point.h"

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Random Number Generation in the range [0, 1)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// Returns a random real number in the range [min, max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline Point random_in_unit_sphere() {
    while (true) {
        auto a = random_double(-1, 1);
        auto b = random_double(-1, 1);
        auto c = random_double(-1, 1);
        Point p(a, b, c);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Point random_in_unit_disk() {
    while (true) {
        // Z is 0 because the lens is a flat 2D disk!
        auto p = Point(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
