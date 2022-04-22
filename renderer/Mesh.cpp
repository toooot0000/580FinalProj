//
// Created by Jerry Ye on 2022/3/24.
//

#include "Mesh.h"

#include <utility>

Util::Color Texture::lookup(double u, double v) const
{
    u = Util::clamp(u, 0., 1.);
    v = Util::clamp(v, 0., 1.);
    int i = std::floor(u * (double)width);
    int j = std::floor(v * (double)height);
    double k1 = u*(double)width - i;
    double k2 = v*(double)height - j;

    Util::Color
        c1 = colorMap[index(i, j)],
        c2 = colorMap[index(i+1, j)],
        c3 = colorMap[index(i, j+1)],
        c4 = colorMap[index(i+1, j+1)];

    c1.scale((1-k1)*(1-k2));
    c2.scale((1-k2)*k1);
    c3.scale(k2*(1-k1));
    c4.scale(k1*k2);

    c1.translate(c2);
    c1.translate(c3);
    c1.translate(c4);

    return c1;
}


Texture &Texture::operator=(Texture &&other) noexcept
{
    width = other.width;
    height = other.height;
    isSet = other.isSet;
    colorMap = std::move(other.colorMap);

    return *this;
}

Texture::Texture(Texture &&other) noexcept
: width(other.width),
    height(other.height),
    colorMap(std::move(other.colorMap)),
    isSet(true)
{}

Texture::Texture(size_t width, size_t height, const std::vector<unsigned char> &image):
        width(width), height(height),
        isSet(true)
{
    for(int i = 0; i<width*height; i++){
        colorMap.emplace_back(image[4*i], image[4*i+1], image[4*i+2], image[4*i+3]);
    }
}
