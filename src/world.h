#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "point3d.h"
#include "hit.h"

class Object;
class Ray;
class Material;

class World {
public:
    void add(std::unique_ptr<Object>&& object);
    std::optional<Hit> find_nearest(const Ray& ray) const; // O(N)

    std::vector<std::unique_ptr<Object>> objects;
};