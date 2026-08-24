#include <iostream>
#include <algorithm>
#include "Point.h"
#include "Ray.h"
#include "Camera.h" 
#include "Shape.h"
#include "Sphere.h"
#include "ShapeList.h"
#include "HitRecord.h"
#include "Material.h"
#include "utils.h"

Point rayColor(const Ray& r, const Shape& world, const int depth = 0) {
    HitRecord rec;
    
    if(depth <= 0) {
        return Point(0, 0, 0);
    }

    if(world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Point attenuation;
        // we check if the material scatters the ray
        if(rec.mat->scatter(r, rec, attenuation, scattered)) {
            // if it does, we return the color of the material multiplied by the color of the scattered ray
            return rayColor(scattered, world, depth - 1) * attenuation;
        }
        return Point(0, 0, 0);
    }

    Point unit_direction = r.direction.unit_vector();
    double t = 0.5 * (unit_direction.y + 1.0);
    return Point(1.0, 1.0, 1.0) * (1.0 - t) + Point(0.5, 0.7, 1.0) * t;
}

void writeColor(std::ostream& out, Point pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Divide the color by the number of samples and apply gamma correction
    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}

int main() {

    // THE CINEMATIC CAMERA
    const double image_width = 1920;
    const double image_height = 1080;
    const auto aspect_ratio = image_width / image_height;

    const int sample_per_pixel = 1000;
    
    // Camera is pulled back, slightly to the left, and low to the ground
    Point lookfrom(-1.8, 0.4, 2.0);
    Point lookat(0.0, 0.0, -1.0);
    Point vup(0, 1, 0);

    auto dist_to_focus = (lookfrom - lookat).length(); 
    auto aperture = 0.05;   // Just enough aperture to gently blur the gold and the horizon
    auto vfov = 22.0;       // A gorgeous telephoto compression

    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);
    //The PPM file header
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

// Pitch dark slate floor to absorb stray light and ground the composition
    auto mat_ground = std::make_shared<Lambertian>(Point(0.05, 0.05, 0.06)); 
    
    // 2. THE MATERIALS
    // The Hero: Brushed 316L Stainless Steel
    auto mat_brushed_steel = std::make_shared<AnisotropicMetal>(Point(0.85, 0.85, 0.85), 0.7, 0.02);
    // The Crystal: Thick, heavy refractive glass
    auto mat_glass = std::make_shared<Dielectric>(1.5);
    // Accent 1: A rich, matte oxblood burgundy
    auto mat_oxblood = std::make_shared<Lambertian>(Point(0.3, 0.05, 0.05));
    // Accent 2: High-polish silver/rhodium for sharp specular highlights
    auto mat_polished = std::make_shared<Metal>(Point(0.95, 0.95, 0.95), 0.01);

    ShapeList world;
    world.add(std::make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, mat_ground));

    // 3. THE ACTORS (Tightly clustered for maximum reflection interaction)
    // Center-Right: The massive brushed steel sphere
    world.add(std::make_shared<Sphere>(Point(0.4, 0.0, -1.2), 0.5, mat_brushed_steel));
    
    // Front-Left: The glass dome, positioned perfectly to distort the steel behind it
    world.add(std::make_shared<Sphere>(Point(-0.2, -0.1, -0.5), 0.4, mat_glass));
    
    // Back-Left: The dark oxblood matte sphere, bleeding color into the glass and steel
    world.add(std::make_shared<Sphere>(Point(-0.8, 0.0, -1.5), 0.5, mat_oxblood));
    
    // Far-Right Foreground: A tiny high-polish silver bearing to catch a sharp pinprick of light
    world.add(std::make_shared<Sphere>(Point(1.1, -0.3, -0.6), 0.2, mat_polished));

    // 4. The Render Loop
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {

            Point pixel_color(0, 0, 0); // we start with black (shadow)
            
            for (int s = 0; s < sample_per_pixel; s++){
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);

                Ray r = camera.getRay(u, v);
                pixel_color = pixel_color + rayColor(r, world, 100);
            }

            writeColor(std::cout, pixel_color, sample_per_pixel);
        }
    }

    return 0;
}
