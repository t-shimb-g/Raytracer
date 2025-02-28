#include "lambertian.h"
#include "random.h"
#include "ray.h"
#include "hit.h"

Lambertian::Lambertian(Color color, bool emitting)
    : Material{"lambertian", color, emitting} {}

Ray Lambertian::scatter(const Ray&, const Hit& hit) const {
    // Random point on sphere
    Point3D sphere_surface_point = (hit.position + hit.normal) + random_unit_vector();

    Vector3D scattered = sphere_surface_point - hit.position;
    return Ray{hit.position, scattered};
}