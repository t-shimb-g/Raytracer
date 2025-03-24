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
#include "glass.h"
#include "random.h"
#include "parser.h"
#include <cmath>
#include <iomanip>
#include <chrono>

using Time = std::chrono::time_point<std::chrono::system_clock>;

void print_progress(long long ray_num, long long total_rays, const Time start);
Color trace(const World& world, const Ray& ray);
Color trace_path(const World& world, const Ray& ray, int depth);
Color iterative_trace_path(const World& world, const Ray& ray, int depth);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {filename}\n";
        return 0;
    }

    Parser parser{argv[1]};
    World world = parser.get_world();
    Pixels pixels = parser.get_pixels();
    Camera camera = parser.get_camera();

    int ray_depth = parser.ray_depth;
    int samples = parser.ray_samples;

    // Track progress
    const long long total_rays = pixels.rows * pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;
    Time render_start = std::chrono::system_clock::now();

    // Initial progress bar print
    print_progress(ray_num, total_rays, render_start);
    Time percent_start = std::chrono::system_clock::now();

    for (auto row = 0; row < pixels.rows; ++row) {
        for (auto col = 0; col < pixels.columns; ++col) {
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays (for antialiasing)
                Ray ray = camera.compute_ray(x, y);
                pixels(row, col) += trace_path(world, ray, ray_depth);

                ++ray_num;
                if (ray_num % (total_rays / 100) == 0) {
                    print_progress(ray_num, total_rays, percent_start);
                    percent_start = std::chrono::system_clock::now();
                }
            }
            pixels(row, col) /= samples;
        }
    }

    Time render_end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration<double>(render_end - render_start);

    pixels.save_png(parser.filename);
    std::cout << "\n\nWrote " << parser.filename << '\t';
    std::cout << "Time: " << duration << '\n';
}

void print_progress(long long ray_num, long long total_rays, const Time start) {
    // Percentage variables
    auto width = std::to_string(total_rays).length() + 4;
    double progress = static_cast<double>(ray_num) / total_rays;
    int bar_width = 30;
    int filled = std::round(progress * bar_width);
    int unfilled = bar_width - filled;

    // Time variables
    double time = std::chrono::duration<double>(std::chrono::system_clock::now() - start).count();
    int total_seconds = (progress > 0) ? static_cast<int>(time * ((100 / progress) - 100)) : 0; // Prevents division by 0
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    // Display percentage and ray number
    std::cout << std::setfill(' ') << "\rProgress: " << std::setw(3) << static_cast<int>(progress * 100.0) << "% ";
    std::cout << std::setw(width) << ray_num << "/" << total_rays << " rays";

    // Display progress bar
    std::cout << " | [" << std::string(filled, '#');
    std::cout << std::string(unfilled, '-') << ']';

    // Display estimated time remaining
    std::cout << " ETR:";
    std::cout << std::setfill(' ') << std::setw(2) << hours << "h " << std::setw(2) << minutes << "m " << std::setw(2) << seconds << "s";

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

Color iterative_trace_path(const World& world, const Ray& ray, int depth) {
    Ray r = ray;
    Color final_color = White;

    // Gather all colors
    while (depth > 0) {
        std::optional<Hit> hit = world.find_nearest(r);
        if (!hit) {
            return Black;
        }

        const Material* material = hit->sphere->material;
        final_color *= material->color;

        if (material->emitting) {
            return final_color;
        }
        r = material->scatter(r, hit.value());

        --depth;
    }

    if (depth == 0) {
        return Black;
    }
    else {
        return final_color;
    }
}