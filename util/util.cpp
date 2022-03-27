//
// Created by Jerry Ye on 2022/3/24.
//

#include "util.h"


float Util::D2Line::operator()(float x, float y) const
{
    return this->A * x + this->B * y + this->C;
}

float Util::D2Line::operator()(const Vec<float, 4>& p) const
{
    return this->operator()(p.getX(), p.getY());
}

float Util::D2Line::operator()(int x, int y) const
{
    return this->operator()(
            static_cast<float>(x),
            static_cast<float>(y)
            );
}

Vec3 Util::Color::toVec3() const
{
    return {(float)r/255.f, (float)g/255.f, (float)b/255.f};
}
