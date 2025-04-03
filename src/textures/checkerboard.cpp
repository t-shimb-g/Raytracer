#include "checkerboard.h"

Checkerboard::Checkerboard(Color a, Color b)
    : a{a}, b{b} {}

Color Checkerboard::value(double u, double v) const {
    int x = static_cast<int>(u * 20) % 2;
    int y = static_cast<int>(v * 10) % 2;
    if ((x + y) % 2 == 0) {
        return a;
    }
    else {
        return b;
    }
}
