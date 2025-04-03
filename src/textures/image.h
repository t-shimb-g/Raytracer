#include "texture.h"
#include "pixels.h"

class Pixels;

class Image : public Texture {
public:
    Image(const std::string& filename);
    Color value(double u, double v) const override;

private:
    Pixels pixels;
};