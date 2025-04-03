add_library(core
    vector3d.h vector3d.cpp
    hit.h hit.cpp
    material.h material.cpp
    ray.h ray.cpp
    random.h random.cpp
    constants.h
    point3d.h
    color.h
    object.h object.cpp
    pcg_random.hpp
    pcg_extras.hpp
    pcg_uint128.hpp
    texture.h texture.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})