//
// Created by Jerry Ye on 2022/3/24.
//

#include "Mesh.h"

#include <utility>


Mesh::Mesh(std::vector<Vec3> vertices, std::vector<Vec3> uvs, std::vector<Vec3> norms,
           std::vector<Tri> tris) : vertices(std::move(vertices)), uvs(std::move(uvs)), norms(std::move(norms)), tris(std::move(tris)) {}

void Mesh::setScale(float scale)
{
    Mesh::scale = scale;
}

void Mesh::setX(float x)
{
    Mesh::x = x;
}

void Mesh::setY(float y)
{
    Mesh::y = y;
}

void Mesh::setZ(float z)
{
    Mesh::z = z;
}

float Mesh::getScale() const
{
    return scale;
}

float Mesh::getX() const
{
    return x;
}

float Mesh::getY() const
{
    return y;
}

float Mesh::getZ() const
{
    return z;
}

float Mesh::getRotateX() const
{
    return rotateX;
}

float Mesh::getRotateY() const
{
    return rotateY;
}

float Mesh::getRotateZ() const
{
    return rotateZ;
}

void Mesh::setRotateX(float rotateX)
{
    Mesh::rotateX = rotateX;
}

void Mesh::setRotateY(float rotateY)
{
    Mesh::rotateY = rotateY;
}

void Mesh::setRotateZ(float rotateZ)
{
    Mesh::rotateZ = rotateZ;
}

float Mesh::getS() const
{
    return s;
}

const Vec3 &Mesh::getKa() const
{
    return ka;
}

const Vec3 &Mesh::getKs() const
{
    return ks;
}

const Vec3 &Mesh::getKd() const
{
    return kd;
}
