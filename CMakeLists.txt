cmake_minimum_required(VERSION 3.25)
project(raytracer)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# adjust any compiler settings
add_compile_options(-flto -O3 -g -p -Wall -Wextra -pedantic -std=c++20 -fno-omit-frame-pointer)

add_subdirectory(src)
add_subdirectory(test)