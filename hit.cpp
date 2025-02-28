#include "hit.h"

Hit::Hit(double time, const Point3D& position, const Vector3D& normal, const Sphere* sphere)
    : time{time}, position{position}, normal{unit(normal)}, sphere{sphere} {}
    // Make sure normal has length 1