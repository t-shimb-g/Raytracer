#pragma once

#include "point3d.h"
#include "vector3d.h"

class Sphere;

class Hit {
public:
    Hit(double time, const Point3D& position, const Vector3D& normal, const Sphere* sphere=nullptr);

    double time;
    Point3D position;
    Vector3D normal;

    const Sphere* sphere;
};