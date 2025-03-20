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
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})