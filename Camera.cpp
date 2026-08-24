#include "Camera.h"

Camera::Camera(Point lookFrom, Point lookAt, Point vup, double vfov, double aspectRatio, double aperture, double focus_dist) {
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspectRatio * viewport_height;

    // camera coordinate system
    auto w = (lookFrom - lookAt).unit_vector();
    auto u = (vup.cross(w)).unit_vector();
    auto v = w.cross(u);

    origin = lookFrom;
    lens_radius = aperture / 2.0;

    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
}

Ray Camera::getRay(double s, double t) const{
    // pick a random point on the lens
    Point rd = lens_radius * random_in_unit_disk();
    Point offset = u * rd.x + v * rd.y;

    return Ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset);
}
