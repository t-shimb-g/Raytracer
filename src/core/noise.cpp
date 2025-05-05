#include "noise.h"
#include "point3d.h"
#include <cmath>
#include <array>

namespace Perlin {
// Ken Perlin's improved noise for the SIGGRAPH 2002 paper

constexpr static std::array<int, 256> permutation = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,
    103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,
    0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149,
    56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166,
    77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187,
    208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
    3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212,
    207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248,
    152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,
    242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107,
    49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
    150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
    215, 61,  156, 180};

consteval std::array<int, 512> fill_p() {
    std::array<int, 512> result = {};
    for (int i = 0; i < 256; ++i) {
        result[i] = result[i+256] = permutation[i];
    }
    return result;
}

constexpr static std::array<int, 512> p = fill_p();


double fade(double t) {
    // Ken's improved quintic interpolation
    return t * t * t * (t * (t * 6 - 15) + 10);
}
double lerp(double t, double a, double b) {
    // linear interpolation
    return a + t * (b - a);
}
double grad(int hash, double x, double y, double z) {
    // convert lowest 4 bits of hash into 12 gradient directions
    int h = hash & 15;
    double u = h<8 ? x : y;
    double v = h<4 ? y : h==12 || h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double noise(double x, double y, double z) {
    // find the unit cube that contains the point (x, y, z)
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;
    
    // find the relative x, y, z of point within cube
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    // compute fade curves for each x, y, z
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);
    
    // hash coordinates of the 8 cube corners
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;  

    // add and blend results from 8 corners of the cube
    return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),
                                   grad(p[BA  ], x-1, y  , z   )),
                           lerp(u, grad(p[AB  ], x  , y-1, z   ), 
                                   grad(p[BB  ], x-1, y-1, z   ))),
                   lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  
                                   grad(p[BA+1], x-1, y  , z-1 )), 
                           lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                   grad(p[BB+1], x-1, y-1, z-1 ))));
}

double turbulence(double x, double y, double z, int depth) {
    double result = 0;
    double weight = 1;
    for (int i = 0; i < depth; ++i) {
        result += weight * noise(x, y, z);
        // Tweak below for different outcomes
        weight *= 0.5;
        x *= 2;
        y *= 2;
        z *= 2;
    }
    return result;
}

} // end namespace Perlin
