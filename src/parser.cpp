#include "parser.h"
#include "color.h"
#include "diffuse.h"
#include "metallic.h"
#include "lambertian.h"
#include "specular.h"
#include "glass.h"
#include "camera.h"
#include "pixels.h"
#include "sphere.h"
#include "triangle.h"
#include "solid.h"
#include "gradient.h"
#include "image.h"
#include "checkerboard.h"
#include "surf_norm.h"
#include "function2d.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

Parser::Parser(const std::string& filename)
    : filename{filename} {
    std::ifstream input{filename};
    if (!input) {
        std::string msg {"Could not open filename: " + filename};
        throw std::runtime_error(msg);
    }
    parse(input);
    verify();
}

void remove_comment(std::string& line) {
    auto i = line.find('#');
    if (i != std::string::npos) { // found a #
        line = line.substr(0, i);
    }
}

bool is_whitespace(const std::string& line) {
    return std::all_of(std::begin(line), std::end(line), ::isspace);
}

void Parser::parse(std::ifstream& input) {
    for (std::string line; std::getline(input, line);) {
        remove_comment(line); // remove all text after #

        if (is_whitespace(line)) {
            continue;
        }
        try {
            std::stringstream ss{line};
            std::string type;
            ss >> type; // gets the first word
            if (type == "material") {
                parse_material(ss);
            }
            else if (type == "camera") {
                parse_camera(ss);
            }
            else if (type == "sphere") {
                parse_sphere(ss);
            }
            else if (type == "triangle") {
                parse_triangle(ss);
            }
            else if (type == "mesh") {
                parse_mesh(ss);
            }
            else if (type == "output") {
                parse_output(ss);
            }
            else if (type == "rays") {
                parse_rays(ss);
            }
            else if (type == "pixels") {
                parse_pixels(ss);
            }
            else if (type == "threads") {
                parse_threads(ss);
            }
            else if (type == "texture") {
                parse_texture(ss);
            }
            else {
                throw std::runtime_error("Unknown type: " + type + " in line: " + line);
            }
        }
        catch (std::runtime_error& e) {
            std::cout << "Error when parsing line:\n" << line;
            throw e;
        }
    }
}

Camera Parser::get_camera() {
    return Camera{camera_position, camera_target, camera_up, camera_fov, aspect};
}

Pixels Parser::get_pixels() {
    return Pixels{columns, rows};
}

World Parser::get_world() {
    return std::move(world);
}

void Parser::verify() {
    std::string msg{"Missing from file: "};
    if (!found_camera) {
        throw std::runtime_error(msg + "camera");
    }
    if (!found_pixels) {
        throw std::runtime_error(msg + "pixels");
    }
    if (world.objects.empty()) {
        throw std::runtime_error(msg + "sphere");
    }
    if (filename.empty()) {
        throw std::runtime_error(msg + "output");
    }
    if (materials.empty()) {
        throw std::runtime_error(msg + "material");
    }
    if (!found_rays) {
        throw std::runtime_error(msg + "rays");
    }
}

void Parser::parse_material(std::stringstream& ss) {
    std::string name, kind, texture_name;
    bool emitting;
    if (!(ss >> name >> kind >> texture_name >> std::boolalpha >> emitting)) {
        throw std::runtime_error("Malformed material");
    }
    const Texture* texture = get_texture(texture_name);

    if (kind == "diffuse") {
        materials[name] = std::make_unique<Diffuse>(texture, emitting);
    }
    else if (kind == "lambertian") {
        materials[name] = std::make_unique<Lambertian>(texture, emitting);
    }
    else if (kind == "metallic") { // Needs fuzz value
        double fuzz;
        if (ss >> fuzz) {
            materials[name] = std::make_unique<Metallic>(texture, emitting, fuzz);
        } // Fuzz exists
        else {
            throw std::runtime_error("Missing parameter 'fuzz' for material: " + name);
        }
    }
    else if (kind == "specular") {
        materials[name] = std::make_unique<Specular>(texture, emitting);
    }
    else if (kind == "glass") {
        materials[name] = std::make_unique<Glass>(texture, emitting);
    }
    else {
        throw std::runtime_error("Unknown material kind: " + name);
    }
}

Material* Parser::get_material(const std::string& material) {
    auto i = materials.find(material);
    if (i != materials.end()) { // Found it!
        return i->second.get();
    }
    else {
        throw std::runtime_error("Unknown material: " + material);
    }
}

void Parser::parse_sphere(std::stringstream& ss) {
    Point3D center;
    double radius;
    std::string material_name;
    if (ss >> center >> radius >> material_name) {
        const Material* material = get_material(material_name);
        std::unique_ptr<Object> object = std::make_unique<Sphere>(center, radius, material);
        world.add(std::move(object));
    }
    else {
        throw std::runtime_error("Malformed sphere");
    }
}

void Parser::parse_triangle(std::stringstream& ss) {
    Point3D v0, v1, v2;
    std::string material_name;
    if (ss >> v0 >> v1 >> v2 >> material_name) {
        const Material* material = get_material(material_name);
        std::unique_ptr<Object> object = std::make_unique<Triangle>(v0, v1, v2, material);
        world.add(std::move(object));
    }
    else {
        throw std::runtime_error("Malformed triangle");
    }
}

void Parser::parse_mesh(std::stringstream& ss) {
    // mesh position filename material_name
    Point3D position;
    std::string filename, material_name;
    if (!(ss >> position >> filename >> material_name)) {
        throw std::runtime_error("Malformed mesh");
    }
    const Material* material = get_material(material_name);

    std::ifstream input{filename};
    if (!input) {
        throw std::runtime_error("Cannot open mesh file :" + filename);
    }
    std::string temp;
    input >> temp;
    if (temp != "vertices") {
        throw std::runtime_error("Mesh file must start with 'vertices'");
    }

    std::vector<Vector3D> vertices;
    for (Vector3D vertex; input >> vertex;) {
        vertices.push_back(vertex + position);
    }
    if (vertices.size() < 3) {
        throw std::runtime_error("Mesh file must contain at least 3 vertices");
    }

    // input attemped to read "triangles" > Vector3D
    input.clear(); // clears error bit
    input >> temp; // "triangles"
    /*
    if (temp != "triangles") {
        throw std::runtime_error("Mesh file must contain 'triangles'");
    }
    */
    // read each line under triangles
    for (int a, b, c; input >> a >> b >> c;) {
        std::unique_ptr<Object> triangle = std::make_unique<Triangle>(vertices.at(a),
                                                                      vertices.at(b),
                                                                      vertices.at(c),
                                                                      material);
        world.add(std::move(triangle));
    }
}

void Parser::parse_camera(std::stringstream& ss) {
    if (ss >> camera_position >> camera_target >> camera_up >> camera_fov) {
        found_camera = true;
    }
    else {
        throw std::runtime_error("Malformed camera");
    }
}

void Parser::parse_rays(std::stringstream& ss) {
    if (ss >> ray_depth >> ray_samples) {
        found_rays = true;
    }
    else {
        throw std::runtime_error("Malformed rays");
    }
}

void Parser::parse_pixels(std::stringstream& ss) {
    if (ss >> columns >> rows) {
        aspect = static_cast<double>(columns) / rows;
        found_pixels = true;
    }
    else {
        throw std::runtime_error("Malformed pixels");
    }
}

void Parser::parse_output(std::stringstream& ss) {
    if (ss >> filename) {
        found_output = true;
    }
    else {
        throw std::runtime_error("Malformed output");
    }
}

void Parser::parse_threads(std::stringstream& ss) {
    if (!(ss >> num_threads)) {
        throw std::runtime_error("Missing parameter for threads");
    }
}

void Parser::parse_texture(std::stringstream& ss) {
    std::string name, kind;
    ss >> name >> kind;
    if (kind == "solid") {
        Color color;
        if (ss >> color) {
            textures[name] = std::make_unique<Solid>(color);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name);
        }
    }
    else if (kind == "gradient") {
        Color a, b;
        if (ss >> a >> b) {
            textures[name] = std::make_unique<Gradient>(a, b);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name);
        }
    }
    else if (kind == "image") {
        std::string filename;
        if (ss >> filename) {
            textures[name] = std::make_unique<Image>(filename);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name);
        }
    }

    else if (kind == "checkerboard") {
        Color a, b;
        if (ss >> a >> b) {
            textures[name] = std::make_unique<Checkerboard>(a, b);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name);
        }
    }
    else if (kind == "surfnorm") {
        textures[name] = std::make_unique<SurfNorm>();
    }
    else if (kind == "stripes") {
        Color a, b;
        if (ss >> a >> b) {
            textures[name] = std::make_unique<Stripes>(a, b);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name);
        }
    }
}

Texture *Parser::get_texture(const std::string &texture) {
    auto i = textures.find(texture);
    if (i != textures.end()) { // Found it!
        return i->second.get();
    }
    else {
        throw std::runtime_error("\nUnknown texture: " + texture);
    }
}
