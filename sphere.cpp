#include "sphere.h"
#include "constants.h"
#include <cmath>

Sphere::Sphere(const Point3D &center, double radius)
    : center{center}, radius{radius} {}

std::optional<double> Sphere::intersect(const Ray &ray) const {
    const Vector3D point_to_center = center - ray.origin;
    const double p2c_squared = dot(point_to_center, point_to_center); // point_to_center squared
    const double ray_proj = dot(ray.direction, point_to_center);
    const double q2 = p2c_squared - (ray_proj * ray_proj);
    const double r2 = radius * radius;

    if (std::abs(q2 - r2) <= Constants::epsilon) { // Single hit!
        return ray_proj;
    }
    if (q2 < radius * radius) { // Double hit!
        const double h = std::sqrt(r2 - q2);

        // ray_proj(ection) is a point inside sphere
        // h = distance to surface of sphere (from ray_proj point)
        // ray_proj - h = closer (first) point on sphere surface <-- the one we want!
        // ray_proj + h = further (second) point on sphere surface

        // Was previously just returning 'ray_proj - h' instead of checking it against epsilon again
        if (ray_proj - h > Constants::epsilon) {
            return ray_proj - h;
        }
        return std::nullopt;
    }
    return std::nullopt; // Miss :(
}

std::optional<double> Sphere::algebraic_intersect(const Ray &ray) const {
    const Vector3D point_to_center = center - ray.origin;
    const double a = dot(ray.direction, ray.direction);
    const double b = 2 * dot(point_to_center, ray.direction);
    const double c = dot(point_to_center, point_to_center) - (radius * radius);

    const double discriminant = (b * b) - (4 * a * c);
    const double t = (b - std::sqrt(discriminant)) / (2 * a);

    if (discriminant > Constants::epsilon) { // Double hit!
        return t; // Quadratic formula
    }
    if (std::abs(discriminant) <= Constants::epsilon) { // Single hit!
        return t;
    }
    return std::nullopt; // Miss :(
}

Hit Sphere::construct_hit(const Ray &ray, double time) const {
    // Calculate the surface normal
    Point3D point = ray.at(time);
    Vector3D normal = (point - center) / radius; // Calling unit() is too expensive - Might come back and bite us
    return Hit{time, point, normal};
}