//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_RAYCAST_H
#define INC_580_FINAL_PROJ_RAYCAST_H

#include "../linear/Vec.h"
#include "Mesh.h"
#include "Tri.h"
#include "../dataStructure/KdTree.h"
#include "../linear/Mat.h"


typedef Tri RayCastBaseTri;
namespace RayCast{
    class Tri: public RayCastBaseTri, public KdTree::ObjectInterface{
    private:
        Vec3 rtf, lbb;
        Vec3 ws;

    public:
        Tri(const Vertex& v1, const Vertex& v2, const Vertex& v3);
        Tri(Vertex&& v1, Vertex&& v2, Vertex&& v3);
        explicit Tri(RayCastBaseTri&&);

        inline void updateBoundary(){
            for(auto i : {0, 1, 2}){
                lbb[i] = std::min({this->operator[](0).position[i], this->operator[](1).position[i], this->operator[](2).position[i]});
                rtf[i] = std::max({this->operator[](0).position[i], this->operator[](1).position[i], this->operator[](2).position[i]});
            }
        }
        [[nodiscard]] inline const Vec3 & getRightTopFront() const override { return rtf; };
        [[nodiscard]] inline const Vec3 & getLeftBottomBack() const override { return lbb; };

        [[nodiscard]] inline const Vec3 &getWs() const { return ws; };
        [[nodiscard]] inline Vec3& getWs() { return ws; };
    };
}

typedef Mesh<RayCast::Tri> RayCastBaseMesh;


namespace RayCast{
    class Ray: public KdTree::RayInterface{
    public:
        Ray(const Vec3&  startPoint, const Vec3&  dir);
        [[nodiscard]] double intersect(const KdTree::ObjectInterface*) const override;
        [[nodiscard]] Vec3 triangleIntersect(const Tri*) const;
    };

    class MeshInterface: public RayCastBaseMesh{
    public:
        MeshInterface(
                const std::vector<Vec3> &vertices,
                const std::vector<Vec3> &uvs,
                const std::vector<Vec3> &norms,
                const std::vector<TriInd> &tris): RayCastBaseMesh(vertices, uvs, norms, tris){};
        virtual void applyTransformation(const Mat4 &toCmr, const Mat4 &nToCmr) = 0;
        [[nodiscard]] virtual const Tri* detectCollision(const Ray&) const = 0;
    };

    class Mesh: public MeshInterface{
    private:
        KdTree *represent = nullptr;

    public:
        Mesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
             const std::vector<TriInd> &tris);
        void applyTransformation(const Mat4 &toCmr, const Mat4 &nToCmr) override;
        [[nodiscard]] const Tri* detectCollision(const Ray&) const override ;

        [[nodiscard]] KdTree *getRepresent() const;

    };

    class PlainMesh: public MeshInterface{
    public:
        PlainMesh(const std::vector<Vec3> &vertices, const std::vector<Vec3> &uvs, const std::vector<Vec3> &norms,
             const std::vector<TriInd> &tris);
        void applyTransformation(const Mat4 &toCmr, const Mat4 &nToCmr) override;
        [[nodiscard]] const Tri* detectCollision(const Ray&) const override;
    };



}



#endif //INC_580_FINAL_PROJ_RAYCAST_H
