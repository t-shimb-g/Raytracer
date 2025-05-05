# test vector3d operations
add_executable(test-vector test-vector3d.cpp)
target_link_libraries(test-vector PRIVATE core)

# test pixel class
add_executable(test-pixels test-pixels.cpp)
target_link_libraries(test-pixels PRIVATE image)

# test ray sphere intersections
add_executable(test-intersect test-intersect.cpp)
target_link_libraries(test-intersect PRIVATE raytracer)

# test random functions
add_executable(test-random test-random.cpp)
target_link_libraries(test-random PRIVATE core)

# test camera
add_executable(test-camera test-camera.cpp)
target_link_libraries(test-camera PRIVATE raytracer)

# test materials
add_executable(test-materials test-materials.cpp)
target_link_libraries(test-materials PRIVATE raytracer)

# test world class
add_executable(test-world test-world.cpp)
target_link_libraries(test-world PRIVATE raytracer)

# test parser
add_executable(test-parser test-parser.cpp)
target_link_libraries(test-parser PRIVATE raytracer)

# test triangle
add_executable(test-triangle test-triangle.cpp)
target_link_libraries(test-triangle PRIVATE raytracer)

# test fog
add_executable(test-fog test-fog.cpp)
target_link_libraries(test-fog PRIVATE raytracer)

# test BVH
add_executable(test-bvh test-bvh.cpp)
target_link_libraries(test-bvh PRIVATE raytracer)

# test BVH intersection
add_executable(test-bvh2 test-bvh2.cpp)
target_link_libraries(test-bvh2 PRIVATE raytracer)

# main
add_executable(main main.cpp)
target_link_libraries(main PRIVATE raytracer)