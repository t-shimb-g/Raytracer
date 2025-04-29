#pragma once

#include "object.h"

class ConstantMedium : public Object {
public:
    ConstantMedium(Object* boundary, double density, const Material* material);
    ~ConstantMedium() override;

    AABB bounding_box() const override;
    std::optional<double> intersect(const Ray& ray) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
    std::pair<double, double> uv(const Hit& hit) const override;

    Object* boundary; // Assume ownership
    double density;
};
