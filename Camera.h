#pragma once
#include "Point.h"
#include "Ray.h"
#include "utils.h"

class Camera {
    private:
        //Camera parameters
        Point origin;
        Point lower_left_corner;
        Point horizontal;
        Point vertical;
        Point u, v, w;
        double lens_radius;

    public:
        //Constructors
        Camera(Point lookFrom, Point lookAt, Point vup, double vfov, double aspectRatio, double aperture, double focus_dist);
        
        //Utility functions
        Ray getRay(double u, double v) const;
};
