#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "constants.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "world.h"
#include "diffuse.h"
#include "lambertian.h"
#include "specular.h"
#include "metallic.h"
#include "random.h"
#include <cmath>
#include <iomanip>

void print_progress(long long ray_num, long long total_rays);
Color trace(const World& world, const Ray& ray);
Color trace_path(const World& world, const Ray& ray, int depth);

int main() {
    // materials
    Lambertian blue{Blue, false};
    Lambertian red{Red, false};
    Specular mirror{White, false};
    Metallic gold{Yellow, false, 0.3};
    Diffuse diff_green{Green, false};

    Diffuse gray{{0.5, 0.5, 0.5}, false};
    Diffuse light{{1, 1, 1}, true};

    // world
    World world;
    world.add({1010, 0, 0}, 1000, &mirror);
    world.add({-1010, 0, 0}, 1000, &mirror);
    world.add({0, 1010, 0}, 1000, &mirror);
    world.add({0, -1010, 0}, 1000, &mirror);
    world.add({0, 0, 1010}, 1000, &light);
    world.add({0, 0, -1010}, 1000, &gray);
    world.add({0, 0, 0}, 1, &red);
    // specify the number of pixels
    Pixels pixels{1920, 1080};

    // create the camera
    Vector3D position{6, 0, 3}, up{0, 0, 1};
    Vector3D target{0, 0, 0};
    double fov{90};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;
    Camera camera{position, target, up, fov, aspect};

    constexpr int samples = 5000;
    constexpr int ray_depth = 20;

    // Track progress
    const long long total_rays = pixels.rows * pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;
    // Initial progress bar print
    print_progress(ray_num, total_rays);

    for (auto row = 0; row < pixels.rows; ++row) {
        for (auto col = 0; col < pixels.columns; ++col) {
            for (int N = 0; N < samples; ++N) {
                double y = (row + random()) / (pixels.rows - 1);
                double x = (col + random()) / (pixels.columns - 1);
                // cast samples number of rays (for antialiasing)
                Ray ray = camera.compute_ray(x, y);
                pixels(row, col) += trace_path(world, ray, ray_depth);

                ++ray_num;
                if (ray_num % (total_rays / 100) == 0) {
                    print_progress(ray_num, total_rays);
                }
            }
            pixels(row, col) /= samples;
        }
    }
    std::string filename{"sphere.png"};
    pixels.save_png(filename);
    std::cout << "\n\nWrote " << filename << '\n';
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    int percentage = std::round(static_cast<double>(ray_num) / total_rays * 100);
    int bar_width = 30;
    int filled = std::round((static_cast<double>(percentage) / 100) * bar_width);
    int unfilled = bar_width - filled;

    // Display percentage and ray number
    std::cout << std::setfill(' ') << "\rProgress: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << "/" << total_rays << " rays | ";

    // Display progress bar
    std::cout << '[' << std::string(filled, '#');
    std::cout << std::string(unfilled, '-') << ']';
    std::cout << std::flush;
}

Color trace(const World& world, const Ray& ray) {
    std::optional<Hit> hit = world.find_nearest(ray);
    if (hit.has_value()) {
        return hit->normal;
    }
    else {
        return Black;
    }
}

Color trace_path(const World& world, const Ray& ray, int depth) {
    if (depth == 0) {
        return Black;
    }

    std::optional<Hit> hit = world.find_nearest(ray);
    if (!hit) {
        return Black; // Artifacts(?)
    }

    const Material* material = hit->sphere->material;
    if (material->emitting) {
        return material->color;
    }

    // more bounces!
    Ray scattered = material->scatter(ray, hit.value());
    return material->color * trace_path(world, scattered, depth-1);
}