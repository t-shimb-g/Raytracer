#pragma once

#include "material.h"

class Metallic : public Material {
public:
    Metallic(const Texture* texture, bool emitting, double fuzz);

    Ray scatter(const Ray& ray, const Hit& hit) const override;

private:
    double fuzz;
};
