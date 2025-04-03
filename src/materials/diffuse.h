#pragma once

#include "material.h"

class Diffuse : public Material {
public:
    Diffuse(const Texture* texture, bool emitting); // Hardcode name

    Ray scatter(const Ray& ray, const Hit& hit) const override;
};
