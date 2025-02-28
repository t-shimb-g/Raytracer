#pragma once

#include "point3d.h"
#include "hit.h"
#include "ray.h"
#include <optional>

class Material;

class Sphere {
public:
    Sphere(const Point3D& center, double radius, const Material* material=nullptr);

    std::optional<double> algebraic_intersect(const Ray& ray) const;
    std::optional<double> intersect(const Ray& ray) const;
    Hit construct_hit(const Ray& ray, double time) const;

    Point3D center;
    double radius;

    const Material* material;
};
