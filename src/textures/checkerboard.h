#include "texture.h"

class Checkerboard : public Texture {
public:
    Checkerboard(Color a, Color b);
    Color value(double u, double v) const override;

private:
    Color a, b;
};