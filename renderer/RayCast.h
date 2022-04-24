//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_RAYCAST_H
#define INC_580_FINAL_PROJ_RAYCAST_H

#include "../linear/Vec.h"
#include "Mesh.h"
#include "Tri.h"
#include "../dataStructure/KdTree.h"


typedef Tri RayCastBaseTri;
namespace RayCast{
    class Tri: public RayCastBaseTri, public KdTree::ObjectInterface{
    private:
        Vec3 rtf, lbb;
    public:
        Tri(const Vertex& v1, const Vertex& v2, const Vertex& v3): RayCastBaseTri(v1, v2, v3){} ;
        Tri(Vertex&& v1, Vertex&& v2, Vertex&& v3): RayCastBaseTri(v1, v2, v3){} ;
        explicit Tri(RayCastBaseTri&&);
        [[nodiscard]] inline Vec3 getRightTopFront() const override { return rtf; };
        [[nodiscard]] inline Vec3 getLeftBottomBack() const override { return lbb; };
    };
}

typedef Mesh<RayCast::Tri> RayCastBaseMesh;


namespace RayCast{
    class Ray: public KdTree::RayInterface{
    public:
        Ray(const Vec3&  startPoint, const Vec3&  dir);
        [[nodiscard]] double intersect(const KdTree::ObjectInterface*) const override;
    };

    class MeshInterface: public RayCastBaseMesh{
    public:
        MeshInterface(
                const std::vector<Vec3> &vertices,
                const std::vector<Vec3> &uvs,
                const std::vector<Vec3> &norms,
                const std::vector<TriInd> &tris): RayCastBaseMesh(vertices, uvs, norms, tris){};
        [[nodiscard]] virtual const Tri* detectCollision(const Ray&) const = 0;
    };

    class Mesh: public MeshInterface{
    private:
        Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
             const std::vector<TriInd> &tris);
        KdTree *represent = nullptr;

    public:
        [[nodiscard]] const Tri* detectCollision(const Ray&) const override ;
    };

    class PlainMesh: public MeshInterface{
    public:
        [[nodiscard]] const Tri* detectCollision(const Ray&) const override;
    };

}



#endif //INC_580_FINAL_PROJ_RAYCAST_H
