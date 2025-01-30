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
    Color temp = {0, 0, 0};

    for (int i = 0; i < pixels.columns; ++i) {
        for (int j = 0; j < pixels.rows; ++j) {
            pixels(j, i) = temp;
        }
        temp += Color{1.0/pixels.columns, 1.0/pixels.columns, 1.0/pixels.columns};
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
    Pixels pixels{columns, rows};

    pretty(pixels);
    std::string filename{"pretty.ppm"};
    pixels.save_ppm(filename);
    std::cout << "Wrote: " << filename << '\n';

    /*
    // example:
    set_color(pixels, White);
    std::string filename{"white.png"};
    pixels.save_png(filename);
    std::cout << "Wrote: " << filename << '\n';
    */
}
