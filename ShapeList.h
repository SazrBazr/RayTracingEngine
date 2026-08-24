#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class ShapeList : public Shape {
    private:
        std::vector<std::shared_ptr<Shape>> shapes;

    public:
        ShapeList() {}

        void add(std::shared_ptr<Shape> shape) {
            shapes.push_back(shape);
        }

        void clear() {
            shapes.clear();
        }

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
            HitRecord temp_rec;
            bool hit_anything = false;
            double closest_so_far = t_max;

            for (const auto& shape : shapes) {
                if (shape->hit(r, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }
};
