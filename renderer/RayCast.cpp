//
// Created by Jerry Ye on 2022/3/30.
//

#include "RayCast.h"
#include "../linear/Mat.h"

#include <utility>

const RayCast::Tri * RayCast::Mesh::detectCollision(const RayCast::Ray &ray) const
{
    if(!represent){
        return nullptr;
    }
    auto ptr = static_cast<const KdTree::RayInterface*>(&ray);
    return dynamic_cast<const Tri*>( represent->traverse(*ptr));
}

RayCast::Mesh::Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
                    const std::vector<TriInd> &tris) : MeshInterface(vertices, uvs, norms, tris)
{
    represent = new KdTree(getTris());
}



RayCast::Ray::Ray(const Vec3& startPoint, const Vec3& dir)
    :RayInterface(startPoint, dir)
{
}

double RayCast::Ray::intersect(const KdTree::ObjectInterface *obj) const
{
    auto tri = dynamic_cast<const Tri*>(obj);
    if(!tri){
        return -1;
    }
    auto &va = (*tri)[0].position, &vb = (*tri)[1].position, &vc = (*tri)[2].position;
    auto &e = getStartPoint(), &dir = getDir();
    Mat3 a{
        va.getX() - vb.getX(), va.getX() - vc.getX(), dir.getX(),
        va.getY() - vb.getY(), va.getY() - vc.getY(), dir.getY(),
        va.getZ() - vb.getZ(), va.getZ() - vc.getZ(), dir.getZ()
    }, b{
        va.getX() - vb.getX(), va.getX() - vc.getX(), va.getX() - e.getX(),
        va.getY() - vb.getY(), va.getY() - vc.getY(), va.getY() - e.getY(),
        va.getZ() - vb.getZ(), va.getZ() - vc.getZ(), va.getZ() - e.getZ()
    };
    return b.determinant()/a.determinant();
}

RayCast::Tri::Tri(RayCastBaseTri && other) : RayCastBaseTri(std::move(other)){
    for(auto i : {0, 1, 2}){
        lbb[i] = std::min({this->operator[](0).position[i], this->operator[](1).position[i], this->operator[](2).position[i]});
        rtf[i] = std::max({this->operator[](0).position[i], this->operator[](1).position[i], this->operator[](2).position[i]});
    }
}

const RayCast::Tri *RayCast::PlainMesh::detectCollision(const RayCast::Ray &ray) const
{
    Tri const* ret = nullptr;
    double curT = INT_MAX;
    for(auto& tri : getTris()){
        auto ptr = dynamic_cast<KdTree::ObjectInterface const*>(&tri);
        double t = ray.intersect(ptr);
        if(t > 0 && t < curT){
            curT = t;
            ret = dynamic_cast<const Tri *>(ptr);
        }
    }
    return ret;
}
