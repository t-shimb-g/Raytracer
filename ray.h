#pragma once

#include "vector3d.h"
#include "point3d.h"

class Ray {
public:
    Ray(const Point3D& origin, const Vector3D& direction);
    Point3D at(double t) const;

    Point3D origin;
    Vector3D direction;
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);