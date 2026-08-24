#pragma once
#include "utils.h"
#include "Ray.h"
#include "HitRecord.h"

class Material {
    public:
        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Point& attenuation, Ray& scattered) const = 0;

        virtual ~Material() = default;
};

class Lambertian : public Material{
    public:
        Point albedo;

        Lambertian(const Point& a) : albedo(a) {}

        bool scatter(const Ray& r_in, const HitRecord& rec, Point& attenuation, Ray& scattered) const override {
            
            Point scatter_direction = rec.normal + random_in_unit_sphere();

            // Catch degenerate scatter direction
            if (scatter_direction.magnitude() < 1e-8)
                scatter_direction = rec.normal;

            scattered = Ray(rec.point, scatter_direction);
            attenuation = albedo; // return the color of the material
            return true;
        }
};

class Metal : public Material {
    public:
        Point albedo;
        double fuzz; // 0 is perfect mirror, 1 is very fuzzy

        Metal(const Point& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        bool scatter(const Ray& r_in, const HitRecord& rec, Point& attenuation, Ray& scattered) const override {
            // we reflect the ray across the normal
            Point reflected = reflect(r_in.direction.unit_vector(), rec.normal);
            // and then we add some fuzziness using the same technique we used for the diffuse material, but we scale it by the fuzz factor
            scattered = Ray(rec.point, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;

            // if the ray is scattered into the material we absorb it
            return (scattered.direction.dot(rec.normal) > 0);
        }

    private:
        Point reflect(const Point& v, const Point& n) const {
            return v - 2 * v.dot(n) * n;
        }
};

class AnisotropicMetal : public Material {
    public:
        Point albedo;
        double fuzz_x;
        double fuzz_y;

        AnisotropicMetal(const Point& a, double fx, double fy) : albedo(a), fuzz_x(fx < 1 ? fx : 1), fuzz_y(fy < 1 ? fy : 1) {}

        bool scatter(const Ray& r_in, const HitRecord& rec, Point& attenuation, Ray& scattered) const override {
            Point reflected = reflect(r_in.direction.unit_vector(), rec.normal);
            Point w = rec.normal;
            Point a = (std::abs(w.x) > 0.9) ? Point(0, 1, 0) : Point(1, 0, 0);
            Point v = w.cross(a).unit_vector();
            Point u = w.cross(v);
            // We create an anisotropic fuzz by scaling the random vector differently in the x and y directions
            Point fuzzy_offset = u * (random_double(-1.0, 1.0) * fuzz_x) + v * (random_double(-1.0, 1.0) * fuzz_y);
            scattered = Ray(rec.point, reflected + fuzzy_offset);
            attenuation = albedo;
            return (scattered.direction.dot(rec.normal) > 0);
        }

    private:
        Point reflect(const Point& v, const Point& n) const {
            return v - 2 * v.dot(n) * n;
        }
};

class Dielectric : public Material {
    public:
        double ir; // Index of Refraction (air = 1.0, glass = 1.5, water = 1.33)

        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        bool scatter(const Ray& r_in, const HitRecord& rec, Point& attenuation, Ray& scattered) const override {
            attenuation = Point(1.0, 1.0, 1.0); // No attenuation for dielectric materials

            Point unit_direction = r_in.direction.unit_vector();

            // check if we are inside or outisde
            Point outward_normal;
            double refraction_ratio;

            // if the ray and the normal face the same direction, it means we are inside the material
            if (unit_direction.dot(rec.normal) > 0) {
                outward_normal = rec.normal * -1.0;
                refraction_ratio = ir; // we are going from the material to the air
            } else {
                outward_normal = rec.normal;
                refraction_ratio = 1.0 / ir; // we are going from the air to the material
            }
            
            // Calculate angles for Snell's Law
            double cos_theta = fmin(-unit_direction.dot(outward_normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            // Check for total internal reflection
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Point direction;

            // Use Schlick's approximation for reflectance to decide whether to reflect or refract
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                // Reflect
                direction = reflect(unit_direction, outward_normal);
            } else {
                // Refract
                direction = refract(unit_direction, outward_normal, refraction_ratio);
            }

            scattered = Ray(rec.point, direction);
            return true;
        }

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Schlick's approximation for reflectance
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }

        Point reflect(const Point& v, const Point& n) const {
            return v - 2.0 * v.dot(n) * n;
        }

        Point refract(const Point& uv, const Point& n, double etai_over_etat) const {
            double cos_theta = fmin((uv.dot(n) * (-1)), 1.0);
            Point r_out_perp = etai_over_etat * (uv + cos_theta * n);
            Point r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.magnitude() * r_out_perp.magnitude())) * n;
            return r_out_perp + r_out_parallel;
        }
};
