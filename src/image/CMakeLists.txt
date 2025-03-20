add_library(image
    lodepng.h lodepng.cpp
    pixels.h pixels.cpp
)

target_include_directories(image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(image PUBLIC core)