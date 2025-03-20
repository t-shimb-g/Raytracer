#include "lambertian.h"
#include "random.h"
#include "ray.h"
#include "hit.h"

Lambertian::Lambertian(Color color, bool emitting)
    : Material{"lambertian", color, emitting} {}

Ray Lambertian::scatter(const Ray&, const Hit& hit) const {
    Vector3D scattered = hit.normal + random_unit_vector();
    return Ray{hit.position, scattered};
}