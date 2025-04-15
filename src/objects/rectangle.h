#pragma once

#include "object.h"
#include "triangle.h"

class Rectangle : public Object {
public:
    Rectangle(const Point3D& origin, const Vector3D& bottom_edge, double height, const Material* material);

    AABB bounding_box() const override;
    std::optional<double> intersect(const Ray &ray) const override;
    Hit construct_hit(const Ray &ray, double time) const override;
    std::pair<double, double> uv(const Hit &hit) const override;

    Point3D origin;
    Point3D p1, p2, p3; // p3 is opposite diagonal from origin
    Vector3D bottom_edge;
    double height;
    Triangle tri_a, tri_b;
};