//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_RAYCAST_H
#define INC_580_FINAL_PROJ_RAYCAST_H

#include "../linear/Vec.h"
#include "Mesh.h"
#include "Tri.h"
#include "../dataStructure/KdTree.h"


typedef Mesh RayCastBaseMesh;
typedef Tri RayCastBaseTri;

namespace RayCast{

    class Tri: RayCastBaseTri, KdTree::ObjectInterface{
    public:
        Tri(RayCastBaseTri&&);
        Vec3 getRightTopFront() const;
        Vec3 getLeftBottomBack() const;
        int compareToOn(const ObjectInterface&, Axis axis);
    };

    class Ray: KdTree::RayInterface{
    private:
        Vec3 startPoint;
        Vec3 dir;
    public:
        Ray(Vec3  startPoint, Vec3  dir);
        [[nodiscard]] Vec3 getStartPoint() const;
        [[nodiscard]] Vec3 getDir() const;
    };

    class Mesh: public RayCastBaseMesh{
    private:
        KdTree *represent = nullptr;
    public:
        Mesh(RayCastBaseMesh&&);
        Tri* detectCollision(const Ray&);
    };
}



#endif //INC_580_FINAL_PROJ_RAYCAST_H
