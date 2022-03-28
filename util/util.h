//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_UTIL_H
#define INC_580_FINAL_PROJ_UTIL_H
#include "../tgaimage/tgaimage.h"
#include "../linear/Vec.h"

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
            explicit Color(const Vec3& v) : TGAColor(
                    static_cast<unsigned char>(v.getX()*255),
                    static_cast<unsigned char>(v.getY()*255),
                    static_cast<unsigned char>(v.getZ()*255),  1){};
            [[nodiscard]]inline Vec3 toVec3() const{
                return {(float)r/255.f, (float)g/255.f, (float)b/255.f};
            };

            inline void scale(float v)
            {
                r = static_cast<unsigned char>(Util::clamp(static_cast<float>(r)*v, 0.f, 255.f));
                g = static_cast<unsigned char>(Util::clamp(static_cast<float>(g)*v, 0.f, 255.f));
                b = static_cast<unsigned char>(Util::clamp(static_cast<float>(b)*v, 0.f, 255.f));
            };
            inline void scale(const Vec3& v)
            {
                r = static_cast<unsigned char>(Util::clamp(static_cast<float>(r)*v.getX(), 0.f, 255.f));
                g = static_cast<unsigned char>(Util::clamp(static_cast<float>(g)*v.getY(), 0.f, 255.f));
                b = static_cast<unsigned char>(Util::clamp(static_cast<float>(b)*v.getZ(), 0.f, 255.f));
            };
            inline void translate(const Vec3& v){
                r = static_cast<unsigned char>(Util::clamp(static_cast<float>(r)/255+v.getX(), 0.f, 1.f)*255);
                g = static_cast<unsigned char>(Util::clamp(static_cast<float>(g)/255+v.getY(), 0.f, 1.f)*255);
                b = static_cast<unsigned char>(Util::clamp(static_cast<float>(b)/255+v.getZ(), 0.f, 1.f)*255);
            };
            inline void translate(const Color& v){
                r += v.r;
                g += v.g;
                b += v.b;
                r = clamp<unsigned char>(r, 0, 255);
                g = clamp<unsigned char>(g, 0, 255);
                b = clamp<unsigned char>(b, 0, 255);
            };
    };

    struct D2Line
    {
        float A{};
        float B{};
        float C{};
        D2Line() = default;

        template<unsigned long L>
        requires(L >= 2)
        D2Line(const Vec<float, L>& v0, const Vec<float, L>& v1){
            this->A = v0[1] - v1[1];
            this->B = v1[0] - v0[0];
            this->C = v0.getX() * v1.getY() - v0.getY() * v1.getX();
        }

        float operator()(const Vec<float, 4>& p) const;
        float operator()(float x, float y) const;
        float operator()(int x, int y) const;
    };
};


#endif //INC_580_FINAL_PROJ_UTIL_H
