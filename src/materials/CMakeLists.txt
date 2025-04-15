add_library(materials
    diffuse.h diffuse.cpp
    lambertian.h lambertian.cpp
    specular.h specular.cpp
    glass.h glass.cpp
    metallic.h metallic.cpp
    isotropic.h isotropic.cpp
)

target_include_directories(materials PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(materials PUBLIC core)