//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_RENDERER_H
#define INC_580_FINAL_PROJ_RENDERER_H

#include "../linear/Vec.h"
#include "../linear/Mat.h"
#include "../util/util.h"
#include <vector>


struct Camera{
    Vec3 eye;
    Vec3 viewUp;
    Vec3 gaze;
};

struct Light{
    Vec3 direction;
    Util::Color color;
};

struct Pixel{
    Util::Color color;
    float z{INT_MAX};
};

class Mesh;

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
    Pixel* pixelBuffer;
    int xRes, yRes;

    void rasterize(const Mesh& mesh, Vec4 v[3], Vec4 norm[3], Vec3 uvs[3]);
    static void sortVertices(Vec4 v[3], Vec4 norm[3], Vec3 uvs[3]);
    Util::Color computeColor(const Mesh& mesh, const Vec3& norm, float u, float v);

    void putPixel(int row, int col, Pixel&& p);

public:
    Renderer() = delete;
    Renderer(int xRes, int yRes);
    ~Renderer();
    void render(const Mesh& mesh);
    void flushToImg(const char* name);
    void addLight(const Light& light);
    void addLight(Light&& light);
    void setCamera(const Camera &camera);
    void setAmbientLight(const Light &ambientLight);
};


#endif //INC_580_FINAL_PROJ_RENDERER_H
