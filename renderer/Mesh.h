//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_MESH_H
#define INC_580_FINAL_PROJ_MESH_H

#include "../linear/Vec.h"
#include <vector>
#include <array>

struct Tri{
    /**
     * Vertices index;
     */
    std::array<int, 3> vers;
    /**
     * UV index;
     */
    std::array<int, 3> uv;
    /**
     * Normal index;
     */
    std::array<int, 3> norm;
};

class Mesh
{
private:

    std::vector<Vec3> vertices;
    std::vector<Vec3> uvs;
    std::vector<Vec3> norms;
    std::vector<Tri> tris;
    Vec3 ka{0.1, 0.1, 0.1}, ks{.3, .3, .3}, kd{.7, .7, .7};
    float s = 32;
    float scale = 1;
    float x = 0, y = 0, z = 0;
    float rotateX = 0, rotateY = 0, rotateZ = 0;

public:
    void* texture = nullptr;

    Mesh(std::vector<Vec3> vertices, std::vector<Vec3> uvs, std::vector<Vec3> norms,
         std::vector<Tri> tris);

    [[nodiscard]] inline const std::vector<Vec3> &getVertices() const {return vertices; };

    [[nodiscard]] inline const std::vector<Vec3> &getUvs() const {return uvs;};

    [[nodiscard]] inline const std::vector<Vec3> &getNorms() const {return norms;};

    [[nodiscard]] inline const std::vector<Tri> &getTris() const {return tris;};


    void setScale(float scale);

    void setX(float x);

    void setY(float y);

    void setZ(float z);

    [[nodiscard]] float getScale() const;

    [[nodiscard]] float getX() const;

    [[nodiscard]] float getY() const;

    [[nodiscard]] float getZ() const;

    [[nodiscard]] float getRotateX() const;

    [[nodiscard]] float getRotateY() const;

    [[nodiscard]] float getRotateZ() const;

    void setRotateX(float rotateX);

    void setRotateY(float rotateY);

    void setRotateZ(float rotateZ);

    float getS() const;

    const Vec3 &getKa() const;

    const Vec3 &getKs() const;

    const Vec3 &getKd() const;

};


#endif //INC_580_FINAL_PROJ_MESH_H
