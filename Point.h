#pragma once
#include <cmath>

class Point{
    public:
        double x;
        double y;
        double z;

        //Constructors
        Point() : x(0), y(0), z(0) {}

        Point(double x, double y, double z): x(x), y(y), z(z) {}

        //Operator overloads
        Point operator+(const Point& p) const {
            return Point(x + p.x, y + p.y, z + p.z);
        }

        Point operator-(const Point& p) const {
            return Point(x - p.x, y - p.y, z - p.z);
        }

        Point operator*(double scalar) const {
            return Point(x * scalar, y * scalar, z * scalar);
        }

        Point operator/(double scalar) const {
            return Point(x / scalar, y / scalar, z / scalar);
        }

        Point operator+(double scalar) const {
            return Point(x + scalar, y + scalar, z + scalar);
        }

        Point operator-(double scalar) const {
            return Point(x - scalar, y - scalar, z - scalar);
        }

        Point operator*(const Point& p) const {
            return Point(x * p.x, y * p.y, z * p.z);
        }

        double magnitude() const {
            return sqrt(x * x + y * y + z * z);
        }

        Point unit_vector() const {
            return *this / this->magnitude();
        }

        double dot(const Point& p) const {
            return x * p.x + y * p.y + z * p.z;
        }

        double angle(const Point& p) const {
            double dotProduct = this->dot(p);
            double magnitudes = this->magnitude() * p.magnitude();
            return acos(dotProduct / magnitudes);
        }

        double length_squared() const {
            return x * x + y * y + z * z;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        Point cross(const Point& p) const {
            return Point(
                y * p.z - z * p.y,
                z * p.x - x * p.z,
                x * p.y - y * p.x
            );
        }
};

inline Point operator*(double scalar, const Point& p) {
    // We can just reuse the math from the operator you already wrote inside the class!
    return p * scalar; 
}

inline Point& operator+=(Point& v1, const Point& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}
