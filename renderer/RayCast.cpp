//
// Created by Jerry Ye on 2022/3/30.
//

#include "RayCast.h"

#include <utility>

RayCast::Tri * RayCast::Mesh::detectCollision(const RayCast::Ray &)
{
    return {};
}

RayCast::Mesh::Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
                    const std::vector<TriInd> &tris) : RayCastBaseMesh(vertices, uvs, norms, tris)
{
    represent = new KdTree(getTris());
}



Vec3 RayCast::Ray::getStartPoint() const
{
    return startPoint;
}

Vec3 RayCast::Ray::getDir() const
{
    return dir;
}

RayCast::Ray::Ray(Vec3 startPoint, Vec3 dir)
: startPoint(std::move(startPoint)), dir(std::move(dir))
{
}

RayCast::Tri::Tri(RayCastBaseTri && other) : RayCastBaseTri(std::move(other)){
    for(auto i : {0, 1, 2}){
        lbb[i] = std::min({this->operator[](0).position[i], this->operator[](1).position[i], this->operator[](2).position[i]});
        rtf[i] = std::max({this->operator[](0).position[i], this->operator[](1).position[i], this->operator[](2).position[i]});
    }
}
