#include "bvh.h"
#include "constants.h"
#include "random.h"
#include "hit.h"
#include <algorithm>

BVH::BVH(const std::vector<std::unique_ptr<Object>>& objects) {
    // Nodes of the BVH only need a pointer to the object, not to maintain ownership.
    // Build a list of raw object pointers
    std::vector<Object*> object_pointers;
    for (const auto& obj : objects) {
        object_pointers.push_back(obj.get());
    }
    root = build_tree(object_pointers);
}

std::optional<Hit> BVH::find_nearest(const Ray& ray) const {
    // Test intersection with the root node
    auto hit_pair = intersect(root, ray);

    // If you hit something, then construct a hit on the object otherwise no hit
    if (hit_pair.second.has_value()) {
        return hit_pair.first->construct_hit(ray, hit_pair.second.value());
    }
    return std::nullopt;
}

BVH::Node* BVH::build_tree(std::vector<Object*> objects) const {
    // Recursively build a tree
    // at leaf nodes, set the node object and bounding box
    // for all others partition and recursively call build_tree

    // make sure to handle surrounding boxes at non-leaf nodes
    Node* n = new Node{};
    if (objects.size() == 1) {
        n->object = objects[0];
        n->box = n->object->bounding_box();
    }
    else {
        auto [left, right] = partition_along_random_axis(objects);
        n->left = build_tree(left);
        n->right = build_tree(right);
        n->box = surrounding_box(n->left->box, n->right->box);
    }
    return n;
}

bool BVH::compare_boxes(Object* a, Object* b, int axis) const {
    // For the given axis (x=0, y=1, z=2) returns whether a's box is less than b's
    switch (axis) {
        case 0:
            return a->bounding_box().minimum.x < b->bounding_box().minimum.x;
        case 1:
            return a->bounding_box().minimum.y < b->bounding_box().minimum.y;
        case 2:
            return a->bounding_box().minimum.z < b->bounding_box().minimum.z;
        default:
            throw std::runtime_error("Invalid axis");
    }
}

BVH::Node::Node()
    : box{{}, {}} {}

std::pair<std::vector<Object*>, std::vector<Object*>> BVH::partition_along_random_axis(std::vector<Object*> objects) const {
    std::vector<Object*> left, right;
    int axis = static_cast<int>(random_double(0, 3)); // Random integer, either 0, 1, or 2

    if (objects.size() == 1) {
        left.push_back(objects[0]);
        right.push_back(objects[0]);
    }
    else if (objects.size() == 2) {
        left.push_back(objects[0]);
        right.push_back(objects[1]);
    }
    else {
        // Sort objects using compare_boxes() along a random axis
        // std::sort also works, std::ranges::sort just doesn't use iterators and takes in the entire objects vector
        std::ranges::sort(objects, [this, axis](Object* a, Object* b) { // fancy lambda function
            return compare_boxes(a, b, axis);
        });

        // Divide objects vector into two vectors
        auto mid_itr = objects.begin() + (objects.size() / 2);
        // insert(insert_position, start_range, end_range)
        left.insert(left.begin(), objects.begin(), mid_itr);
        right.insert(right.begin(), mid_itr, objects.end());
    }
    return {left, right};
}

std::pair<Object*, std::optional<double>> BVH::intersect(Node* n, const Ray& ray) const {
    // First handle the ray not intersection the box, then handle leaf nodes
    if (!n->box.intersect(ray)) {
        return {n->object, std::nullopt};
    }
    else if (n->left == nullptr || n->right == nullptr) {
        // Leaf node! return object and intersection
        return {n->object, n->object->intersect(ray)};
    }
    
    // Remember that boxes can overlap, so you must test both left and right boxes.
    // Given the results of hitting possible left or right or both, return the closest
    auto left = intersect(n->left, ray);
    auto right = intersect(n->right, ray);

    bool left_hit = left.second.has_value();
    bool right_hit = right.second.has_value();

    if (left_hit && right_hit) {
        return left.second.value() < right.second.value() ? left : right;
    }
    // If left_hit is false, right_hit has to be true
    return left_hit ? left : right;
}