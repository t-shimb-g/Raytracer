#pragma once

#include "vector3d.h"

double random_double();
double random_double(double min, double max);
double old_random_double(); // [0 - 1)
double old_random_double(double min, double max);

Vector3D random_unit_vector();
Vector3D random_hemisphere(const Vector3D& normal);