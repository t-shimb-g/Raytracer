#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "world.h"
#include "random.h"
#include "parser.h"
#include "bvh.h"
#include <iomanip>
#include <cmath>
#include <chrono>
#include <thread>

using Time = std::chrono::time_point<std::chrono::system_clock>;

void print_progress(long long ray_num, long long total_rays, const Time start);
Color trace(const World& world, const Ray& ray);
Color trace_path(const BVH& bvh, const Ray& ray, int depth);
void render(const BVH& bvh, const Camera& camera, int depth, int samples, int num_threads, Pixels& pixels,
            bool progress=false);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {filename}\n";
        return 0;
    }

    try {
        Parser parser{argv[1]};
        World world = parser.get_world();
        BVH bvh{world.objects};

        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();

        int depth = parser.ray_depth;
        int samples = parser.ray_samples;
        int num_threads = parser.num_threads;
        Time render_start = std::chrono::system_clock::now();

        // Create n-1 images for the additional threads we will launch
        std::vector<Pixels> images;
        for (int i = 0; i < num_threads-1; ++i) {
            images.push_back(pixels);
        }

        // launch additional threads
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads-1; ++i) {
            std::thread t{render, std::ref(bvh), camera, depth, static_cast<double>(samples)/num_threads, num_threads,
                std::ref(images.at(i)), false};
            threads.push_back(std::move(t));
        }

        // render on main thread
        render(bvh, camera, depth, static_cast<double>(samples)/num_threads, num_threads, pixels, true);

        // wait for other threads to finish
        for (std::thread& t : threads) {
            t.join();
        }

        // collect all image data
        for (const Pixels& p : images) {
            for (std::size_t i = 0; i < p.values.size(); ++i) {
                pixels.values.at(i) += p.values.at(i);
            }
        }

        // normalize color values by number of threads
        for (Color& c : pixels.values) {
            c /= num_threads;
        }

        Time render_end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration<double>(render_end - render_start);

        pixels.save_png(parser.filename);
        std::cout << "\n\nWrote " << parser.filename << '\t';
        std::cout << "Time: " << duration << '\n';
    }
    catch (std::exception& err) {
        std::cout << err.what() << '\n';
    }
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

Color trace_path(const BVH& bvh, const Ray& ray, int depth) {
    if (depth == 0) {
        return Black;
    }

    std::optional<Hit> hit = bvh.find_nearest(ray);
    if (!hit) {
        return Black;
    }

    const Object* object = hit->object;
    auto [u, v] = object->uv(*hit);
    const Material* material = object->material;
    Color color = material->texture->value(u, v);
    if (material->emitting) {
        return color;
    }

    // more bounces!
    Ray scattered = material->scatter(ray, hit.value());
    return color * trace_path(bvh, scattered, depth-1);
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

void render(const BVH& bvh, const Camera& camera, int depth, int samples, int num_threads, Pixels& pixels,
            bool progress) {

    // track progress
    const long long rays_total = pixels.rows * pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;
    Time render_start = std::chrono::system_clock::now();

    if (progress) {
        print_progress(ray_num*num_threads, rays_total*num_threads, render_start);
    }

    // Initial progress bar print
    Time percent_start = std::chrono::system_clock::now();

    for (auto row = 0; row < pixels.rows; ++row) {
	    for (auto col = 0; col < pixels.columns; ++col) {
            Color color{0, 0, 0};
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays
                Ray ray = camera.compute_ray(x, y);
                color += trace_path(bvh, ray, depth);

                ++ray_num;
                if (progress && ray_num % (rays_total / 100) == 0) {
                    print_progress(ray_num*num_threads, rays_total*num_threads, percent_start);
                    percent_start = std::chrono::system_clock::now();
                }
            }
            pixels(row, col) = color / samples;
	    }
    }
}