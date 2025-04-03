#include "specular.h"
#include "ray.h"
#include "hit.h"
#include "random.h"

Specular::Specular(const Texture* texture, bool emitting)
    : Material{"specular", texture, emitting} {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = reflect(ray.direction, hit.normal);
    return Ray{hit.position, scattered};
}