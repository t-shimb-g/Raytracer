#include "ray.h"
#include "sphere.h"
#include "pixels.h"
#include <iostream>
#include <optional>
#include <chrono>

int main() {
    // Test ray and sphere intersections!
    Ray test_ray{{0, 0, 0}, {0, 1, 0}};
    std::cout << test_ray << "\n";
    for (int i = 0; i < 3; ++i) {
        auto p = test_ray.at(i);
        std::cout << "Distance from ray origin: " << (p - test_ray.origin) << '\n';
    }

    Sphere test_sphere {Point3D {0, 2, 0}, 1};
    std::optional<double> test_time = test_sphere.intersect(test_ray);
    if (test_time.has_value()) {
        std::cout << "Hit at time value: " << test_time.value() << '\n';
    }
    else { // std::nullopt
        std::cout << "nothing\n";
    }

    // Time both intersection methods!
    Ray time_ray {{0, 0, 0}, {0, 1, 0}};
    Sphere miss_sphere {{7, 0, 10}, 1};
    Sphere single_hit_sphere {{0, 2, -1}, 1};
    Sphere double_hit_sphere {{0, 2, 0}, 1};

    auto start_algebraic = std::chrono::system_clock::now();
    for (int i = 0; i < 10000; ++i) {
        auto time = double_hit_sphere.algebraic_intersect(time_ray);
    }
    auto end_algebraic = std::chrono::system_clock::now();
    std::cout
        << "\nAlgebraic time (10000 rays): "
        << std::chrono::duration_cast<std::chrono::microseconds>(end_algebraic - start_algebraic)
        << " (microseconds)\n";

    auto start_geometric = std::chrono::system_clock::now();
    for (int i = 0; i < 10000; ++i) {
        auto time = double_hit_sphere.intersect(time_ray);
    }
    auto end_geometric = std::chrono::system_clock::now();
    std::cout
        << "\nGeometric time (10000 rays): "
        << std::chrono::duration_cast<std::chrono::microseconds>(end_geometric - start_geometric)
        << " (microseconds)\n";

    // Make an image!
    int rows = 720;
    int columns = 1280;
    Pixels pixels{columns, rows};
    Sphere sphere{{0, 2, 0}, 300};
    for (int row = 0; row < rows; ++row) {
        double z = row - (rows/2);
        for (int col = 0; col < columns; ++col) {
            double x = col - (columns/2);
            Ray ray {{x, 0, z}, {0, 1, 0}};
            auto time = sphere.intersect(ray);
            if (time.has_value()) {
                Hit hit = sphere.construct_hit(ray, time.value());
                pixels(row, col) = -hit.normal;
            }
        }
    }
    pixels.save_png("test_intersect.png");
}