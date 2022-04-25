//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_RENDERER_H
#define INC_580_FINAL_PROJ_RENDERER_H

#include "../linear/Vec.h"
#include "../linear/Mat.h"
#include "../util/util.h"
#include <vector>
#include "RayCast.h"

struct Camera{
    Vec3 eye;
    Vec3 viewUp;
    Vec3 gaze;
};

struct Light{
    Vec3 direction;
    Util::Color color;

    Light(const Vec3& direction, Util::Color color);
};

struct Pixel{
    Util::Color color;
    double z{-2};
};

template<class T>
requires std::derived_from<T, Tri>
class Mesh;

typedef Pixel* PixelBuffer;

//template<size_t AA_SIZE = 1>
class Renderer
{
private:
    Camera camera {
            Vec3{-100, 0, 5},
            Vec3{0, 1, 0},
            Vec3{1, 0, 0}
    };
    std::vector<Light> lights;
    Light ambientLight{Vec3{0, 0, 0}, Util::Color({.3, .3, .3})};
    PixelBuffer pixelBuffer;
    PixelBuffer tempBuffer;
    int xRes, yRes;
    std::vector<std::tuple<double, double, double>> aaSetting{std::tuple{0, 0, 1}};
    void rasterize(const Mesh<> &mesh, std::array<Vec4, 3> &v, std::array<Vec4, 3> &norm, std::array<Vec3, 3> &uvs,
                   PixelBuffer buffer, double xOff, double yOff);
    static void sortVertices(std::array<Vec4, 3> &v, std::array<Vec4, 3> &n, std::array<Vec3, 3> &uvs);
    Util::Color computeColor(const Mesh<>& mesh, const Vec3& norm, double u, double v);
    Util::Color
    computeColor(const RayCast::MeshInterface &mesh, const RayCast::Tri &hitTri, const Vec3 &hitPoint,
                 const Vec3 &norm, double u, double v);
    void putPixel(PixelBuffer buffer, int row, int col, Pixel&& p) const;
    void clearBuffer(PixelBuffer buffer) const;

public:
    Renderer() = delete;
    Renderer(int xRes, int yRes);
    Renderer(int xRes, int yRes, std::vector<std::tuple<double, double, double>> aaSetting);
    ~Renderer();
    void render(const Mesh<>& mesh);
    void rayCastRender(RayCast::MeshInterface &mesh);
    void flushToImg(const char* name);
    void addLight(const Light& light);
    void addLight(Light&& light);
    void setCamera(const Camera &camera);
    void setAmbientLight(const Light &ambientLight);
    void clearBuffer();
};


#endif //INC_580_FINAL_PROJ_RENDERER_H
