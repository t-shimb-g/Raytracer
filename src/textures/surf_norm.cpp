#include "surf_norm.h"
#include "constants.h"

SurfNorm::SurfNorm() = default;

Color SurfNorm::value(double u, double v) const {
    double phi = u * 2 * Constants::pi;
    double theta = v * Constants::pi;

    double r = std::cos(phi) * std::sin(theta);
    double g = std::sin(phi) * std::sin(theta);
    double b = std::cos(theta);

    return {r, g, b};
}
