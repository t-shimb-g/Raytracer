#include "metallic.h"
#include "random.h"
#include "hit.h"
#include "ray.h"

Metallic::Metallic(Color color, bool emitting, double fuzz)
    : Material{"metallic", color, emitting}, fuzz{fuzz} {}

Ray Metallic::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D reflection = reflect(ray.direction, hit.normal);
    Point3D fuzz_point {(hit.position + reflection) + (random_unit_vector() * fuzz)};
    Vector3D scattered = fuzz_point - hit.position;
    return Ray{hit.position, scattered};
}