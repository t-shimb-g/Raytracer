#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "texture.h"
#include "solid.h"
#include "lambertian.h"
#include "world.h"
#include "random.h"
#include "parser.h"
#include "bvh.h"
#include <iomanip>
#include <cmath>
#include <thread>

int main() {
    Ray ray = {{0, 0, 0}, {1, 0, 0}};

    Sphere sphere {{5, 0, 0}, 1, nullptr};
    Solid texture {{1, 0, 0},};
    Lambertian material {(&texture), false};
    auto ptr = std::make_unique<Sphere>(Sphere{{5, 0, 0}, 1, nullptr});
    std::vector<std::unique_ptr<Object>> vector;
    vector.push_back(std::move(ptr));

    BVH bvh2{vector};

    auto hit = bvh2.find_nearest(ray);

    std::cout << std::boolalpha << hit.has_value() << '\n';
    if (hit.has_value()) {
        std::cout << hit.value().time;
    }
    else {
        std::cout << "No hit\n";
    }
}