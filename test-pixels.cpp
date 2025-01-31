#include "pixels.h"
#include "color.h"

// write a function to set all the pixels to the same color
void set_color(Pixels& pixels, Color color) {
    for (int i = 0; i < pixels.rows; ++i) {
        for (int j = 0; j < pixels.columns; ++j) {
            pixels(i, j) = color;
        }
    }
}

// write a function that produces a single color gradient
void gradient(Pixels& pixels) {
    for (int i = 0; i < pixels.columns; ++i) {
        for (int j = 0; j < pixels.rows; ++j) {
            pixels(j, i) += Color{(j * 1.0) / pixels.columns, (j * 1.0) / pixels.columns, (j * 1.0) / pixels.columns};
        }
    }
}

// write a function to color a pretty picture!
void pretty(Pixels& pixels) {
    double b = 0;

    for (int i = 0; i < pixels.columns; ++i) {
        double r = 1;
        double g = 0;
        for (int j = 0; j < pixels.rows; ++j) {
            pixels(j, i) += Color {r, g, b};
            r -= 1.0/pixels.rows;
            g += 1.0/pixels.rows;
        }
        b += 1.0/pixels.columns;
    }
}

int main() {
    const unsigned columns = 1280;
    const unsigned rows = 720;

    Pixels solid_pixels{columns, rows};
    set_color(solid_pixels, Navy);
    std::string solid_filename{"navy.png"};
    solid_pixels.save_png(solid_filename);
    std::cout << "Wrote: " << solid_filename << '\n';

    Pixels gradient_pixels{columns, rows};
    gradient(gradient_pixels);
    std::string gradient_filename{"gradient.png"};
    gradient_pixels.save_png(gradient_filename);
    std::cout << "Wrote: " << gradient_filename << '\n';

    Pixels pretty_pixels{columns, rows};
    pretty(pretty_pixels);
    std::string pretty_filename{"pretty.png"};
    pretty_pixels.save_png(pretty_filename);
    std::cout << "Wrote: " << pretty_filename << '\n';
}
