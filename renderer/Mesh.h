//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_MESH_H
#define INC_580_FINAL_PROJ_MESH_H

#include "../linear/Vec.h"
#include "../util/util.h"
#include <vector>
#include <array>
#include "Tri.h"


struct TriInd{
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

    [[nodiscard]] inline size_t index(size_t i, size_t j) const{
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

template<class T = Tri>
requires std::derived_from<T, Tri>
class Mesh
{
private:

    std::vector<T> tris;

    Vec3 ka{0.1, 0.1, 0.1}, ks{.3, .3, .3}, kd{.7, .7, .7};
    double s = 50;
    double scale = 1;
    double x = 0, y = 0, z = 0;
    double rotateX = 0, rotateY = 0, rotateZ = 0;
    Texture texture;

public:

    Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
         const std::vector<TriInd>& tris);

    Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
         const std::vector<TriInd>& tris, Texture && texture);

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;

    [[nodiscard]] inline const std::vector<T> &getTris() const {return tris;};


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


template<class T>
requires std::derived_from<T, Tri>
Mesh<T>::Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms, const std::vector<TriInd>& tris)
{
    for(const auto& ind : tris){
        Vertex v1{
                vertices[ind.vers[0] - 1],
                norms[ind.norm[0] - 1].normalized(),
                uvs[ind.uv[0] - 1]
        }, v2{
                vertices[ind.vers[1] - 1],
                norms[ind.norm[1] - 1].normalized(),
                uvs[ind.uv[1] - 1]
        }, v3{
                vertices[ind.vers[2] - 1],
                norms[ind.norm[2] - 1].normalized(),
                uvs[ind.uv[2] - 1]
        };
        this->tris.emplace_back(T{std::move(v1), std::move(v2), std::move(v3)});
    }
}
template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setScale(double scale)
{
    Mesh::scale = scale;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setX(double x)
{
    Mesh::x = x;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setY(double y)
{
    Mesh::y = y;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setZ(double z)
{
    Mesh::z = z;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getScale() const
{
    return scale;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getX() const
{
    return x;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getY() const
{
    return y;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getZ() const
{
    return z;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getRotateX() const
{
    return rotateX;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getRotateY() const
{
    return rotateY;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getRotateZ() const
{
    return rotateZ;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setRotateX(double rotateX)
{
    Mesh::rotateX = rotateX;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setRotateY(double rotateY)
{
    Mesh::rotateY = rotateY;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setRotateZ(double rotateZ)
{
    Mesh::rotateZ = rotateZ;
}

template<class T>
requires std::derived_from<T, Tri>
double Mesh<T>::getS() const
{
    return s;
}

template<class T>
requires std::derived_from<T, Tri>
const Vec3 &Mesh<T>::getKa() const
{
    return ka;
}

template<class T>
requires std::derived_from<T, Tri>
const Vec3 &Mesh<T>::getKs() const
{
    return ks;
}

template<class T>
requires std::derived_from<T, Tri>
const Vec3 &Mesh<T>::getKd() const
{
    return kd;
}

template<class T>
requires std::derived_from<T, Tri>
void Mesh<T>::setTexture(Texture &&texture)
{
    this->texture = std::move(texture);
}

template<class T>
requires std::derived_from<T, Tri>
Mesh<T>::Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms, const std::vector<TriInd>& tris,
              Texture &&texture)
        :texture(std::move(texture))
{
    for(const auto& ind : tris){
        Vertex v1{
                vertices[ind.vers[0] - 1],
                norms[ind.norm[0] - 1].normalized(),
                uvs[ind.uv[0] - 1]
        }, v2{
                vertices[ind.vers[1] - 1],
                norms[ind.norm[1] - 1].normalized(),
                uvs[ind.uv[1] - 1]
        }, v3{
                vertices[ind.vers[2] - 1],
                norms[ind.norm[2] - 1].normalized(),
                uvs[ind.uv[2] - 1]
        };
        this->tris.emplace_back(Tri{std::move(v1), std::move(v2), std::move(v3)});
    }
}

template<class T>
requires std::derived_from<T, Tri>
const Texture &Mesh<T>::getTexture() const
{
    return texture;
}

template<class T>
requires std::derived_from<T, Tri>
Mesh<T>::Mesh(Mesh &&other) noexcept
:
    texture(other.texture),
    tris(std::move(other.tris)),
    ka(std::move(other.ka)),
    kd(std::move(other.kd)),
    ks(std::move(other.ks)),
    s(other.s),
    x(other.x),
    y(other.y),
    z(other.y),
    scale(other.scale),
    rotateX(other.rotateX),
    rotateY(other.rotateY),
    rotateZ(other.rotateZ)
{}

#endif //INC_580_FINAL_PROJ_MESH_H
