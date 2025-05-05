#pragma once

#include "object.h"
#include "sphere.h"

class Vector3D;

class Terrain : public Object {
public:
    Terrain(Sphere* boundary, const Material* material, int noise_depth);
    ~Terrain() override;

    AABB bounding_box() const override;
    std::optional<double> intersect(const Ray& ray) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
    std::pair<double, double> uv(const Hit& hit) const override;
    double sdf(const Vector3D point) const;
    double elevation(const Vector3D point) const;

    Sphere* boundary; // Assume ownership
    int noise_depth;
};
