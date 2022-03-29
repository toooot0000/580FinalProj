//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_MESH_H
#define INC_580_FINAL_PROJ_MESH_H

#include "../linear/Vec.h"
#include "../util/util.h"
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


struct Texture{
    size_t width{}, height{};
    std::vector<Util::Color> colorMap;
    bool isSet = false;

    inline size_t index(size_t i, size_t j) const{
        return (height - j - 1) * width + i;
    }


    Texture()= default;
    ~Texture() = default;

    Texture(const Texture& other) = delete;
    Texture &operator=(const Texture& other) = delete;

    Texture(Texture&& other) noexcept;
    Texture &operator=(Texture &&other) noexcept;

    explicit Texture(size_t width, size_t height, const std::vector<unsigned char> &image);
    [[nodiscard]] Util::Color lookup(double u, double v) const;

};

class Mesh
{
private:

    std::vector<Vec3> vertices;
    std::vector<Vec3> uvs;
    std::vector<Vec3> norms;
    std::vector<Tri> tris;
    Vec3 ka{0.1, 0.1, 0.1}, ks{.3, .3, .3}, kd{.7, .7, .7};
    double s = 50;
    double scale = 1;
    double x = 0, y = 0, z = 0;
    double rotateX = 0, rotateY = 0, rotateZ = 0;
    Texture texture;

public:

    Mesh(std::vector<Vec3> vertices, std::vector<Vec3> uvs, std::vector<Vec3> norms,
         std::vector<Tri> tris);

    Mesh(std::vector<Vec3> vertices, std::vector<Vec3> uvs, std::vector<Vec3> norms,
         std::vector<Tri> tris, Texture && texture);


    [[nodiscard]] inline const std::vector<Vec3> &getVertices() const {return vertices; };

    [[nodiscard]] inline const std::vector<Vec3> &getUvs() const {return uvs;};

    [[nodiscard]] inline const std::vector<Vec3> &getNorms() const {return norms;};

    [[nodiscard]] inline const std::vector<Tri> &getTris() const {return tris;};


    void setScale(double scale);

    void setX(double x);

    void setY(double y);

    void setZ(double z);

    [[nodiscard]] double getScale() const;

    [[nodiscard]] double getX() const;

    [[nodiscard]] double getY() const;

    [[nodiscard]] double getZ() const;

    [[nodiscard]] double getRotateX() const;

    [[nodiscard]] double getRotateY() const;

    [[nodiscard]] double getRotateZ() const;

    void setRotateX(double rotateX);

    void setRotateY(double rotateY);

    void setRotateZ(double rotateZ);

    [[nodiscard]] double getS() const;

    [[nodiscard]] const Vec3 &getKa() const;

    [[nodiscard]] const Vec3 &getKs() const;

    [[nodiscard]] const Vec3 &getKd() const;

    void setTexture(Texture &&texture);

    [[nodiscard]] const Texture &getTexture() const;

};


#endif //INC_580_FINAL_PROJ_MESH_H
