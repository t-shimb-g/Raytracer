#include "triangle.h"
#include "ray.h"
#include "constants.h"
#include "hit.h"
#include "aabb.h"

Triangle::Triangle(const Point3D& vertex0, const Point3D& vertex1, const Point3D& vertex2, const Material* material)
    : Object{material}, vertex0{vertex0}, vertex1{vertex1}, vertex2{vertex2},
    edge1{vertex1 - vertex0}, edge2{vertex2 - vertex0},
    normal{unit(cross(edge1, edge2))} {}


AABB Triangle::bounding_box() const {
    double xmin = std::min(vertex0.x, std::min(vertex1.x, vertex2.x));
    double xmax = std::max(vertex0.x, std::max(vertex1.x, vertex2.x));

    double ymin = std::min(vertex0.y, std::min(vertex1.y, vertex2.y));
    double ymax = std::max(vertex0.y, std::max(vertex1.y, vertex2.y));

    double zmin = std::min(vertex0.z, std::min(vertex1.z, vertex2.z));
    double zmax = std::max(vertex0.z, std::max(vertex1.z, vertex2.z));

    Point3D min{xmin, ymin, zmin}, max{xmax, ymax, zmax};
    return AABB{min, max};
}

std::optional<double> Triangle::intersect(const Ray& ray) const {
    // Moeller-Trumbore triangle intersection
    Vector3D h = cross(ray.direction, edge2);
    double a = dot(edge1, h);
    if (a > -Constants::epsilon && a < Constants::epsilon) {
        // ray is parallel to the triangle
        return std::nullopt;
    }

    // Calculate u
    double f = 1.0 / a; // Normalizing factor
    Vector3D s = ray.origin - vertex0;
    double u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        // ray is outside of u coordinates of [0, 1]
        return std::nullopt;
    }

    // Calculate v
    Vector3D q = cross(s, edge1);
    double v = f * dot(ray.direction, q);
    if (v < 0.0 || (u + v) > 1.0) {
        // ray is outside of v coordinates of [0, 1]
        return std::nullopt;
    }

    // Calculate t
    double t = f * dot(edge2, q);
    if ( t > Constants::epsilon) {
        return t; // Hit!
    }
    else {
        return std::nullopt; // ray origin too close to triangle surface
    }
}

Hit Triangle::construct_hit(const Ray& ray, double time) const {
    Point3D point = ray.at(time);
    // ensure that the ray and surface normals are pointing in opposite directions
    bool negative = std::signbit(dot(ray.direction, normal));
    if (negative) {
        // Same direction
        return Hit{time, point, normal, this};
    }
    else {
        return Hit{time, point, -normal, this};
    }
}

std::pair<double, double> Triangle::uv(const Hit& hit) const {
    Vector3D P = hit.position - vertex0;
    double u = dot(P, edge1) / length(edge1);
    double v = dot(P, edge2) / length(edge2);
    double magnitude = (u + v) / 2.0;
    return {u/magnitude, v/magnitude};
}