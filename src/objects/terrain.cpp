#include "terrain.h"
#include "hit.h"
#include "ray.h"
#include "constants.h"
#include "random.h"
#include "aabb.h"
#include "noise.h"
#include "sphere.h"
#include <cmath>

Terrain::Terrain(Sphere* boundary, const Material *material, int noise_depth)
    : Object{material}, boundary{boundary}, noise_depth{noise_depth} {}

Terrain::~Terrain() {
    if (boundary) {
        delete boundary;
    }
}

std::optional<double> Terrain::intersect(const Ray &ray) const {
    auto t1 = boundary->intersect(ray);
    if (!t1) {
        return std::nullopt;
    }

    Vector3D little_further = ray.at(t1.value() + 2*Constants::epsilon);
    auto t2 = boundary->intersect(Ray{little_further, ray.direction});

    if (t2) { // Started outside
        t2.value() += t1.value(); // Add initial hit distance onto second
    }
    else { // Started inside, no t2 hit, t1 + 2*epsilon is outside
        t2 = t1;
        t1 = 0;
    }

    double distance_inside_boundary = t2.value() - t1.value();

    constexpr int num_steps = 1000;
    double distance_traveled = 0;
    for (int i = 0; i < num_steps; ++i) {
        if (distance_traveled > distance_inside_boundary) {
            // Past possible intersection distance
            return std::nullopt;
        }

        Vector3D point = ray.at(distance_traveled);
        if (length(boundary->center - point) < boundary->radius) {
            // Inside sphere, push it out
            auto sphere_normal = unit(point - boundary->center);
            point += sphere_normal * 0.001;
        }

        double sdf_result = sdf(point);
        distance_traveled += sdf_result;
        if (sdf_result < 0.001) {
            // Close enough to warrant a hit
            return distance_traveled + distance_inside_boundary;
            // Idk why adding distance_inside_boundary here works...
        }
    }
    return std::nullopt;
}

double Terrain::sdf(const Vector3D point) const {
    return length(boundary->center - point) - boundary->radius + elevation(point);
}

double Terrain::elevation(const Vector3D point) const {
    return (Perlin::turbulence(point.x, point.y, point.z, noise_depth) + 1) * 0.5;
}

AABB Terrain::bounding_box() const {
    return boundary->bounding_box();
}

std::pair<double, double> Terrain::uv(const Hit& hit) const {
    return boundary->uv(hit);
}

Hit Terrain::construct_hit(const Ray& ray, double time) const {
    // Gradient normals | Central differences
    // From iquilezles.org/articles/normalsSDF/
    Point3D position = ray.at(time);
    auto x =
        sdf(position + Vector3D{Constants::epsilon, 0, 0})
        - sdf(position - Vector3D{Constants::epsilon, 0, 0});
    auto y =
        sdf(position + Vector3D{0, Constants::epsilon, 0})
        - sdf(position - Vector3D{0, Constants::epsilon, 0});
    auto z =
        sdf(position + Vector3D{0, 0, Constants::epsilon})
        - sdf(position - Vector3D{0, 0, Constants::epsilon});
    auto normal = unit(Vector3D{x, y, z});
    return Hit{time, position, normal, this};
}