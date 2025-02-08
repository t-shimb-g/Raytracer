#include "ray.h"

Ray::Ray(const Point3D& origin, const Vector3D& direction)
    : origin{origin}, direction{unit(direction)} {}

Point3D Ray::at(double t) const {
    return origin + direction * t;
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << "Ray: [" << ray.origin << ' ' << ray.direction << ']';
    return os;
}