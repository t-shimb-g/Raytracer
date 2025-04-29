#include "constant_medium.h"
#include "hit.h"
#include "ray.h"
#include "constants.h"
#include "random.h"
#include "aabb.h"
#include <cmath>
ConstantMedium::ConstantMedium(Object* boundary, double density, const Material* material)
    : Object{material}, boundary{boundary}, density{density} {}

ConstantMedium::~ConstantMedium() {
    if (boundary) {
        delete boundary;
    }
}

std::optional<double> ConstantMedium::intersect(const Ray& ray) const {
    auto t1 = boundary->intersect(ray);
    if (!t1) {
        return std::nullopt;
    }

    Vector3D little_further = ray.at(t1.value() + 2*Constants::epsilon);
    auto t2 = boundary->intersect(Ray{little_further, ray.direction});

    // Case 1
    // t1 && t2
    // t2 += t1

    // Case 2:
    // t1 && ! t2
    // t2 == t1
    // t1 = 0

    if (t2) { // Started outside
        t2.value() += t1.value(); // Add initial hit distance onto second
    }
    else { // Started inside, no t2 hit, t1 + 2*epsilon is outside
        t2 = t1;
        t1 = 0;
    }

    double distance_inside_boundary = t2.value() - t1.value();
    double hit_distance = -1/density * std::log(1 - random_double());

    if (hit_distance > distance_inside_boundary) {
        return std::nullopt;
    }

    double time = t1.value() + hit_distance;
    return time;
}

AABB ConstantMedium::bounding_box() const {
    return boundary->bounding_box();
}

std::pair<double, double> ConstantMedium::uv(const Hit& hit) const {
    return boundary->uv(hit);
}

Hit ConstantMedium::construct_hit(const Ray& ray, double time) const {
    Point3D position = ray.at(time);
    return Hit{time, position, -ray.direction, this}; //
}