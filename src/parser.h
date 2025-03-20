#pragma once

#include <string>
#include <map>
#include <memory>
#include "world.h"

class Camera;
class Pixels;
class Material;

using Materials = std::map<std::string, std::unique_ptr<Material>>;

class Parser {
public:
    Parser(const std::string& filename);

    Camera get_camera();
    Pixels get_pixels();
    World get_world();

    std::string filename;
    int ray_depth, ray_samples;

private:

    void parse(std::ifstream& input);
    void verify();

    void parse_material(std::stringstream& ss);
    Material* get_material(const std::string& material);
    void parse_sphere(std::stringstream& ss);
    void parse_camera(std::stringstream& ss);
    void parse_rays(std::stringstream& ss);
    void parse_pixels(std::stringstream& ss);
    void parse_output(std::stringstream& ss);

    World world;

    Materials materials;

    Point3D camera_position, camera_target;
    Vector3D camera_up;
    double camera_fov, aspect;
    bool found_camera;

    bool found_pixels;
    int rows, columns;

    bool found_rays;
    bool found_output;
};
