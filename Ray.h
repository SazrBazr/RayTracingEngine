#pragma once
#include "Point.h"

class Ray{
    public:
        Point origin;
        Point direction;

        //Constructors
        Ray() : origin(Point()), direction(Point()) {}
        Ray(const Point& origin, const Point& direction): origin(origin), direction(direction) {}

        Point at(double t) const {
            return origin + direction * t;
        }
};
