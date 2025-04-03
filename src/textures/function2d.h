#include "texture.h"

class Stripes : public Texture {
public:
    Stripes(Color a, Color b);
    Color value(double u, double v) const override;

private:
    Color a, b;
};
