#pragma once
#include "Point.h"
#include "Ray.h"
#include <memory>

class Material;

struct HitRecord {
    Point point;
    Point normal;
    std::shared_ptr<Material> mat;
    double t;
};
