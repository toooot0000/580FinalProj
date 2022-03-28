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

//Vec3 Util::Color::toVec3() const
//{
//    return {(float)r/255.f, (float)g/255.f, (float)b/255.f};
//}

//void Util::Color::scale(const Vec3 &v)
//{
//    r = static_cast<unsigned char>(Util::clamp(static_cast<float>(r)*v.getX(), 0.f, 255.f));
//    g = static_cast<unsigned char>(Util::clamp(static_cast<float>(g)*v.getY(), 0.f, 255.f));
//    b = static_cast<unsigned char>(Util::clamp(static_cast<float>(b)*v.getZ(), 0.f, 255.f));
//}

//void Util::Color::translate(const Vec3 &v)
//{
//    r = static_cast<unsigned char>(Util::clamp(static_cast<float>(r)/255+v.getX(), 0.f, 1.f)*255);
//    g = static_cast<unsigned char>(Util::clamp(static_cast<float>(g)/255+v.getY(), 0.f, 1.f)*255);
//    b = static_cast<unsigned char>(Util::clamp(static_cast<float>(b)/255+v.getZ(), 0.f, 1.f)*255);
//}

//void Util::Color::translate(const Util::Color &v)
//{
//    r += v.r;
//    g += v.g;
//    b += v.b;
//    r = clamp<unsigned char>(r, 0, 255);
//    g = clamp<unsigned char>(g, 0, 255);
//    b = clamp<unsigned char>(b, 0, 255);
//}


