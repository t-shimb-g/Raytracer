#include "pixels.h"
#include "lodepng.h"
#include <fstream>
#include <cmath>

Pixels::Pixels(int columns, int rows)
    : columns{columns}, rows{rows}, values(columns * rows) {
}

const Color& Pixels::operator()(int row, int col) const {
    // JMB: missing bounds checking for row, col
    // It's possible to be a valid index with row = 0, col = 2000
    if (row < rows && col < columns) {
        return values.at(row * columns + col);
    }
    std::string msg {"Pixel coordinate (" + std::to_string(col) + ", " + std::to_string(row) + ')'};
    msg += " is out of bounds. Valid bounds: (0-" + std::to_string(columns) + ", 0-" + std::to_string(rows) + ')';
    throw std::out_of_range(msg);
}

Color& Pixels::operator()(int row, int col) {
    if (row < rows && col < columns) {
        return values.at(row * columns + col);
    }
    std::string msg {"Pixel coordinate (" + std::to_string(col) + ", " + std::to_string(row) + ')'};
    msg += " is out of bounds. Valid bounds: (0-" + std::to_string(columns) + ", 0-" + std::to_string(rows) + ')';
    throw std::out_of_range(msg);
}

void Pixels::save_ppm(const std::string& filename) {
    // use to_color<unsigned>(color.x) for printing RGB values to file
    // write the pixel values to a file using the NetBPM ppm image format
    std::ofstream output(filename);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }
    output << "P3\n" << columns << ' ' << rows << '\n' << "255\n";
    for (auto color : values) {
        output << to_color<unsigned>(color.x) << ' ' << to_color<unsigned>(color.y) << ' ' << to_color<unsigned>(color.z) << '\n';
    }
}

void Pixels::save_png(const std::string& filename) {
    // lodepng expects pixels to be in a vector of unsigned char, where
    // the elements are R, G, B, alpha, R, G, B, alpha, etc.  Use
    // to_color<unsigned char>(color.x) for putting colors in this
    // vector, and lodepng::encode(filename, vector, width, height)
    // (make sure you check the error code returned by this function
    // using lodepng_error_text(unsigned error)).
    std::ofstream output(filename);
    if(!output) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }
    std::vector<unsigned char> data;
    for (auto c : values) {
        data.push_back(to_color<unsigned char>(c.x));
        data.push_back(to_color<unsigned char>(c.y));
        data.push_back(to_color<unsigned char>(c.z));
        data.push_back(255);
    }
    unsigned error = lodepng::encode(filename, data, columns, rows);
    if (error) {
        throw std::runtime_error(lodepng_error_text(error));
    }
}

double gamma_correction(double value) {
    double gamma = 2.2;
    // double gamma = 1.0;
    return std::pow(value, 1 / gamma);
}
