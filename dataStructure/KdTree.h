//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_KDTREE_H
#define INC_580_FINAL_PROJ_KDTREE_H

#include "../renderer/RayCast.h"
#include "../linear/Vec.h"
#include <vector>
#include <tuple>
#include <algorithm>


class KdTree
{
public:
    struct ObjectInterface{
        enum Axis{
            X, Y, Z, NO
        };
        [[nodiscard]] virtual Vec3 getTopRightFront() const = 0;
        [[nodiscard]] virtual Vec3 getBottomLeftBack() const = 0;
        virtual int compareToOn(const ObjectInterface&, Axis axis) = 0;
    };

    struct RayInterface{
        virtual bool isIntersect(const Vec3& TLF, const Vec3& BRB) = 0;
    };

private:

    const int PartitionResolution = 5;

    struct KdNode{
        // TopLeftFront, BottomRightBack
        Vec3 RTB, LBF;
        ObjectInterface::Axis axis;
        double val;
        KdNode *leftChild, *rightChild;
        std::vector<ObjectInterface* const> triPtrList;


    };

    struct PartitionResult{

        ObjectInterface::Axis axis;
        double val;
        std::vector<ObjectInterface* const> l, r;

    };

    std::unique_ptr<KdNode> root;

    static KdNode* buildTree(std::vector<ObjectInterface* const> objs, const Vec3 &RTF, const Vec3 &LBB);

    static PartitionResult makePartition(std::vector<ObjectInterface* const>& obj, const Vec3 &RTB, const Vec3 &LBF);

public:

    explicit KdTree(std::vector<ObjectInterface>&) ;

    std::vector<ObjectInterface* const> traverse(const RayInterface&) ;

};


#endif //INC_580_FINAL_PROJ_KDTREE_H
