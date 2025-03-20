#include "material.h"
#include <cmath>

Material::Material(std::string name, Color color, bool emitting)
    : name{name}, color{color}, emitting{emitting} {}

std::ostream& operator<<(std::ostream& os, const Material& material) {}

Vector3D reflect(const Vector3D& v, const Vector3D& normal) {
    return v - 2*dot(v, normal) * normal;
}

Vector3D refract(const Vector3D& v, const Vector3D& normal, double index_ratio) {
    double cos_theta = -dot(normal, v);
    double discriminant = 1 - std::pow(index_ratio, 2) * (1 - std::pow(cos_theta, 2));
    if (discriminant >= 0) { // Refraction
        return index_ratio * v + (index_ratio * cos_theta - std::sqrt(discriminant)) * normal;
    }
    else { // Internal reflection
        return reflect(v, normal);
    }
}

double schlick(double cos_theta, double index_ratio) {
    double r0 = std::pow((1 - index_ratio) / (1 + index_ratio), 2);
    return r0 + (1 - r0) * std::pow(1 - cos_theta, 5);
}