// Sphere.h
#pragma once
#include "Shape.h"
#include "Point.h"
#include <memory>

class Sphere : public Shape {
    private:
        Point center;
        double radius;
        std::shared_ptr<Material> mat;

    public:
        Sphere(Point center, double radius, std::shared_ptr<Material> material) : center(center), radius(radius), mat(material) {}

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
            // First we need to calculate the Vectore from the spphere's center to the ray's origin
            Point oc = r.origin - center;

            // Now we need to calculate the cuadratic formula parts
            double a = r.direction.dot(r.direction);
            double half_b = oc.dot(r.direction); // We can ignore the 2 in the formula because it will be canceled out later
            double c = oc.dot(oc) - radius * radius;

            //The Discriminant is the part under the square root in the quadratic formula, and it tells us if there are real solutions
            double discriminant = half_b * half_b - a * c;

            // If the discriminant is negative, then there is no real solution
            if (discriminant < 0) {
                return false;
            }

            //if it has a solution, we need to find it, we do this calculation after the if to decrease the number of the unnecessary calculations
            double sqrt_discriminant = sqrt(discriminant);
            double nearest_root = (-half_b - sqrt_discriminant) / (a);

            if(nearest_root < t_min || nearest_root > t_max) {
                // If the nearest root is not in the acceptable range, we try the other root
                nearest_root = (-half_b + sqrt_discriminant) / (a);
                if(nearest_root < t_min || nearest_root > t_max) {
                    return false;
                }
            }

            // If we are here, it means we have a valid root, so we fill the hit record
            rec.t = nearest_root;
            rec.point = r.at(rec.t);
            rec.normal = (rec.point - center) / radius; // The normal is the vector from the center to the hit point, normalized
            rec.mat = mat;

            return true;
        }
};
