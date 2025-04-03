#include "function2d.h"

Stripes::Stripes(Color a, Color b)
    : a{a}, b{b} {}

Color Stripes::value(double u, double v) const {
    int x = static_cast<int>(v * 10) % 2;
    if (x % 2 == 0) {
        return a;
    }
    else {
        return b;
    }
}