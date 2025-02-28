#include "specular.h"
#include "ray.h"
#include "hit.h"
#include "random.h"

Specular::Specular(Color color, bool emitting)
    : Material{"specular", color, emitting} {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = reflect(ray.direction, hit.normal);
    return Ray{hit.position, scattered};
}