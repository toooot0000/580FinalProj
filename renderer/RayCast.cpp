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
{}

void RayCast::Mesh::buildTree(const Mat4& toCmr, const Mat4& nToCmr)
{
    for(auto& tri : getTris()){
        std::array<Vec4, 3>
            v{
                Vec4(tri[0].position, 1),
                Vec4(tri[1].position, 1),
                Vec4(tri[2].position, 1)
            }, norm{
                Vec4(tri[0].normal, 1),
                Vec4(tri[1].normal, 1),
                Vec4(tri[2].normal, 1)
        };
        std::array<Vec3, 3> uv{
            tri[0].uvw,
            tri[1].uvw,
            tri[2].uvw
        };

//  Apply the transformation
        v[0] = std::move(toCmr * v[0]);
        v[1] = std::move(toCmr * v[1]);
        v[2] = std::move(toCmr * v[2]);

        v[0].homogenize();
        v[1].homogenize();
        v[2].homogenize();

        v[0][3] = std::abs(1 / v[0][3]);
        v[1][3] = std::abs(1 / v[1][3]);
        v[2][3] = std::abs(1 / v[2][3]);

        Vec3
                ws{v[0][3], v[1][3], v[2][3]},
                us{uv[0][0], uv[1][0], uv[2][0]},
                vs{uv[0][1], uv[1][1], uv[2][1]};

        us = ws * us;
        vs = ws * vs;

        uv[0][0] = us[0];
        uv[1][0] = us[1];
        uv[2][0] = us[2];

        uv[0][1] = vs[0];
        uv[1][1] = vs[1];
        uv[2][1] = vs[2];

        norm[0] = std::move(nToCmr * norm[0]);
        norm[1] = std::move(nToCmr * norm[1]);
        norm[2] = std::move(nToCmr * norm[2]);


        for(auto i : {0, 1, 2}){
            for(auto j : {0, 1, 2}){
                tri[i].position[j] = v[i][j];
                tri[i].normal[j] = norm[i][j];
            }
            for(auto j : {0, 1}){
                tri[i].uvw[j] = uv[i][j];
            }
        }
    }
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
    auto bct = triangleIntersect(tri);
    auto b = bct[0], c = bct[1], t = bct[2];
    auto a = 1 - b - c;
    if(a > 0 && b > 0 && c > 0){
        return t;
    } else {
        return -1;
    }
}

Vec3 RayCast::Ray::triangleIntersect(const RayCast::Tri *tri) const
{
    const auto
        &va = (*tri)[0].position,
        &vb = (*tri)[1].position,
        &vc = (*tri)[2].position;
    const auto &e = getStartPoint(), &dir = getDir();
    Mat3 a{
            va.getX() - vb.getX(), va.getX() - vc.getX(), dir.getX(),
            va.getY() - vb.getY(), va.getY() - vc.getY(), dir.getY(),
            va.getZ() - vb.getZ(), va.getZ() - vc.getZ(), dir.getZ()
    }, d{
            va.getX() - vb.getX(), va.getX() - vc.getX(), va.getX() - e.getX(),
            va.getY() - vb.getY(), va.getY() - vc.getY(), va.getY() - e.getY(),
            va.getZ() - vb.getZ(), va.getZ() - vc.getZ(), va.getZ() - e.getZ()
    }, b{
            va.getX() - e.getX(), va.getX() - vc.getX(), dir.getX(),
            va.getY() - e.getY(), va.getY() - vc.getY(), dir.getY(),
            va.getZ() - e.getZ(), va.getZ() - vc.getZ(), dir.getZ()
    }, c{
            va.getX() - vb.getX(), va.getX() - e.getX(), dir.getX(),
            va.getY() - vb.getY(), va.getY() - e.getY(), dir.getY(),
            va.getZ() - vb.getZ(), va.getZ() - e.getZ(), dir.getZ()
    };
    double A = a.determinant();
    return {
        b.determinant()/A, c.determinant()/A, d.determinant()/A
    };
}

RayCast::Tri::Tri(RayCastBaseTri && other) : RayCastBaseTri(std::move(other)){
    updateBoundary();
}

RayCast::Tri::Tri(const Vertex &v1, const Vertex &v2, const Vertex &v3) : RayCastBaseTri(v1, v2, v3)
{
    updateBoundary();
}

RayCast::Tri::Tri(Vertex&& v1, Vertex&& v2, Vertex&& v3) : RayCastBaseTri(v1, v2, v3)
{
    updateBoundary();
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
