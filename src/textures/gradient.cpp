#include "gradient.h"

Gradient::Gradient(Color a, Color b)
    : a{a}, b{b} {}

Color Gradient::value(double u, double v) const {
    return ((1 - v) * a) + (v * b);
}

