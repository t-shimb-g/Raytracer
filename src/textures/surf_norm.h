#include "texture.h"

class SurfNorm : public Texture{
public:
    SurfNorm();
    Color value(double u, double v) const override;
};