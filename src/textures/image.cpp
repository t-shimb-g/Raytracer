#include "image.h"

Image::Image(const std::string &filename)
    : pixels{filename} {}

Color Image::value(double u, double v) const {
    int col = static_cast<int>(u * (pixels.columns - 1));
    int row = static_cast<int>(v * (pixels.rows - 1));
    int index = row * pixels.columns + col;
    return pixels.values.at(index);
}


