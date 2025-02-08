#pragma once

#include "point3d.h"
#include "ray.h"
#include "hit.h"
#include <optional>

class Sphere {
public:
    Sphere(const Point3D& center, double radius);

    std::optional<double> algebraic_intersect(const Ray& ray) const;
    std::optional<double> intersect(const Ray& ray) const;
    Hit construct_hit(const Ray& ray, double time) const;

    Point3D center;
    double radius;
};
