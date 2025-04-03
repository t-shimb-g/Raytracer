#pragma once

#include "point3d.h"
#include "hit.h"
#include "object.h"
#include <optional>

class Ray;
class Material;

class Sphere : public Object{
public:
    Sphere(const Point3D& center, double radius, const Material* material=nullptr);

    std::optional<double> algebraic_intersect(const Ray& ray) const;
    std::optional<double> intersect(const Ray& ray) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
    std::pair<double, double> uv(const Hit& hit) const override;

    Point3D center;
    double radius;
};
