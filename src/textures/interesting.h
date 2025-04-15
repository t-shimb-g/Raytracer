#pragma once

#include "texture.h"

class Interesting : public Texture {
public:
    Interesting();
    Color value(double u, double v) const override;
};
