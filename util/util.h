//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_UTIL_H
#define INC_580_FINAL_PROJ_UTIL_H
#include "../tgaimage/tgaimage.h"
#include "../linear/Vec.h"
#include <iostream>

namespace Util
{

    template <typename T>
    static T max(T a, T b) {
        return (a > b ? a : b);
    }

    template<typename T, typename...P>
    static inline T max(T t, P...args) {
        return (t > max<T>(args...) ? t : max<T>(args...));
    }

    template <typename T>
    static inline T min(T a, T b) {
        return (a > b ? b : a);
    }

    template<typename T, typename...P>
    static T min(T t, P...args) {
        return (t < min<T>(args...) ? t : min<T>(args...));
    }

    template<typename T>
    static T clamp(T v, T l, T r) {
        return min(max(v, l), r);
    }


    class Color : public TGAColor{
        public:
            Color() : TGAColor(){};
            Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
                :TGAColor(r, g, b, a)
            {};
            explicit Color(const Vec3& v) : TGAColor(
                    static_cast<unsigned char>(clamp<double>(v.getX(), 0, 1)*255),
                    static_cast<unsigned char>(clamp<double>(v.getY(), 0, 1)*255),
                    static_cast<unsigned char>(clamp<double>(v.getZ(), 0, 1)*255),
                    1){};

            [[nodiscard]]inline Vec3 toVec3() const{
                return {(double)r/255., (double)g/255., (double)b/255.};
            };

            inline void scale(double v)
            {
                r = static_cast<unsigned char>(Util::clamp(static_cast<double>(r)*v, 0., 255.));
                g = static_cast<unsigned char>(Util::clamp(static_cast<double>(g)*v, 0., 255.));
                b = static_cast<unsigned char>(Util::clamp(static_cast<double>(b)*v, 0., 255.));
                a = static_cast<unsigned char>(Util::clamp(static_cast<double>(a)*v, 0., 255.));
            };
            inline void scale(const Vec3& v)
            {
                r = static_cast<unsigned char>(Util::clamp(static_cast<double>(r)*v.getX(), 0., 255.));
                g = static_cast<unsigned char>(Util::clamp(static_cast<double>(g)*v.getY(), 0., 255.));
                b = static_cast<unsigned char>(Util::clamp(static_cast<double>(b)*v.getZ(), 0., 255.));
                a = static_cast<unsigned char>(Util::clamp(static_cast<double>(a)*v.getZ(), 0., 255.));
            };
            inline void translate(const Vec3& v){
                r = static_cast<unsigned char>(Util::clamp(static_cast<double>(r)/255+v.getX(), 0., 1.)*255);
                g = static_cast<unsigned char>(Util::clamp(static_cast<double>(g)/255+v.getY(), 0., 1.)*255);
                b = static_cast<unsigned char>(Util::clamp(static_cast<double>(b)/255+v.getZ(), 0., 1.)*255);
            };
            inline void translate(const Color& v){
                r += v.r;
                g += v.g;
                b += v.b;
                a += v.a;
                r = clamp<unsigned char>(r, 0, 255);
                g = clamp<unsigned char>(g, 0, 255);
                b = clamp<unsigned char>(b, 0, 255);
                a = clamp<unsigned char>(a, 0, 255);
            };
            inline Color interp(const Color& other, double ratio){
                auto _r = (unsigned char)((double)r*(1-ratio) + (double)other.r),
                _g = (unsigned char)((double)g*(1-ratio) + (double)other.g),
                _b = (unsigned char)((double)b*(1-ratio) + (double)other.b),
                _a = (unsigned char)((double)a*(1-ratio) + (double)other.a)
                ;
                return {_r, _g, _b, _a};
            }
    };

    struct D2Line
    {
        double A{};
        double B{};
        double C{};
        D2Line() = default;

        template<unsigned long L>
        requires(L >= 2)
        D2Line(const Vec<double, L>& v0, const Vec<double, L>& v1){
            this->A = v0[1] - v1[1];
            this->B = v1[0] - v0[0];
            this->C = v0.getX() * v1.getY() - v0.getY() * v1.getX();
        }

        double operator()(const Vec<double, 4>& p) const;
        double operator()(double x, double y) const;
        double operator()(int x, int y) const;
    };
};


#endif //INC_580_FINAL_PROJ_UTIL_H
