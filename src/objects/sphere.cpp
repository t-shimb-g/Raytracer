#include "sphere.h"
#include "ray.h"
#include "constants.h"
#include <cmath>

Sphere::Sphere(const Point3D &center, double radius, const Material* material)
    : Object{material}, center{center}, radius{radius} {}

std::optional<double> Sphere::intersect(const Ray &ray) const {
    const Vector3D point_to_center = center - ray.origin;
    const double p2c_squared = dot(point_to_center, point_to_center); // point_to_center squared
    const double ray_proj = dot(ray.direction, point_to_center);
    const double q2 = p2c_squared - (ray_proj * ray_proj);
    const double r2 = radius * radius;

    if (std::abs(q2 - r2) <= Constants::epsilon) { // Single hit!
        return ray_proj;
    }
    if (q2 < r2) { // Double hit!
        const double h = std::sqrt(r2 - q2);

        // ray_proj(ection) is a point inside sphere
        // h = distance to surface of sphere (from ray_proj point)
        // ray_proj - h = closer (first) point on sphere surface [return if ray origin is OUTSIDE sphere]
        // ray_proj + h = further (second) point on sphere surface [return if ray origin is INSIDE sphere
        if (length(point_to_center) < radius) { // Inside sphere!
            return ray_proj + h;
        }
        if (ray_proj - h < 0) { // Somehow this can end up being negative, and that's bad
            return std::nullopt;
        }
        return ray_proj - h; // Outside sphere!
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
    return Hit{time, point, normal, this};
}

std::pair<double, double> Sphere::uv(const Hit& hit) const {
    double theta = std::acos(hit.normal.z); // [0, pi]
    double phi = std::atan2(hit.normal.y, hit.normal.x); // [-pi, pi]

    double u = 0.5 + phi / (2 * Constants::pi); // [0, 1]
    double v = theta / Constants::pi; // [0, 1]

    return {u, v};
}