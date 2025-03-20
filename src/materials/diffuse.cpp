#include "diffuse.h"
#include "random.h"
#include "hit.h"
#include "ray.h"

Diffuse::Diffuse(Color color, bool emitting)
    : Material{"diffuse", color, emitting} {}

Ray Diffuse::scatter(const Ray& ray, const Hit &hit) const {
    Vector3D scattered = random_hemisphere(hit.normal);
    return Ray{hit.position, scattered};
}