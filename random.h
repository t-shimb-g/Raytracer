#pragma once

#include "vector3d.h"

double random(); // [0 - 1)
double random(double min, double max);

Vector3D random_unit_vector();
Vector3D random_hemisphere(const Vector3D& normal);