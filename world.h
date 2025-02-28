#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "point3d.h"
#include "hit.h"

class Sphere;
class Ray;
class Material;

class World {
public:
    void add(Point3D center, double radius, const Material* material=nullptr);
    std::optional<Hit> find_nearest(const Ray& ray) const; // O(N)

    std::vector<std::unique_ptr<Sphere>> objects;
};