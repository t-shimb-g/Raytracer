#include "metallic.h"
#include "sphere.h"
#include "random.h"

Metallic::Metallic(Color color, bool emitting, double fuzz)
    : Material{"metallic", color, emitting}, fuzz{fuzz} {}

Ray Metallic::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D reflection = reflect(ray.direction, hit.normal);
    Sphere fuzz_sphere {hit.position + reflection, fuzz};
    // Random point on fuzz sphere
    Point3D fuzz_point {fuzz_sphere.center + (random_unit_vector() * fuzz)};
    /*
    // Possible to not create an entire sphere object
    // Just pass in the logic for the sphere's center instead
    Point3D fuzz_point {(hit.position + reflection) + (random_unit_vector() * fuzz)};
    */
    Vector3D scattered = fuzz_point - hit.position;
    return Ray{hit.position, scattered};
}