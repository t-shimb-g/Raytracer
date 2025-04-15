#include "rectangle.h"
#include "hit.h"
#include "aabb.h"

Rectangle::Rectangle(const Point3D& origin, const Vector3D& bottom_edge, double height, const Material* material)
    : Object{material}, origin{origin}, bottom_edge{bottom_edge}, height{height},
    p1{origin + bottom_edge}, p2{origin + Vector3D{0, 0, height}}, p3{p1 + Vector3D{0, 0, height}},
    tri_a{origin, p1, p2, material}, tri_b{p1, p2, p3, material} {}

std::optional<double> Rectangle::intersect(const Ray &ray) const {
    if (auto t = tri_a.intersect(ray)) {
        return t;
    }
    else {
        return tri_b.intersect(ray);
    }
}

AABB Rectangle::bounding_box() const {
    double xmin = std::min(origin.x, p3.x);
    double xmax = std::max(origin.x, p3.x);

    double ymin = std::min(origin.y, p3.y);
    double ymax = std::max(origin.y, p3.y);

    double zmin = std::min(origin.z, p3.z);
    double zmax = std::max(origin.z, p3.z);

    Point3D min{xmin, ymin, zmin}, max{xmax, ymax, zmax};
    return AABB{min, max};
}

Hit Rectangle::construct_hit(const Ray& ray, double time) const {
    if (auto t = tri_a.intersect(ray)) {
        return tri_a.construct_hit(ray, t.value());
    }
    else {
        t = tri_b.intersect(ray);
        return tri_b.construct_hit(ray, t.value());
    }
}

// TODO: Make a functioning uv() function for special texture support
std::pair<double, double> Rectangle::uv(const Hit& hit) const {
    return {0, 0};
}