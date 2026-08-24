#pragma once
#include "Ray.h"
#include "HitRecord.h"

class Shape {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;

        virtual ~Shape() = default;
};
