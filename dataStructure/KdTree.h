//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_KDTREE_H
#define INC_580_FINAL_PROJ_KDTREE_H

#include "../linear/Vec.h"
#include <vector>
#include <tuple>
#include <algorithm>


class KdTree
{
private:
    struct KdNode;

public:
    struct ObjectInterface{
        enum Axis{
            X = 0, Y = 1, Z = 2, NO = -1
        };
        [[nodiscard]] virtual Vec3 getRightTopFront() const = 0;
        [[nodiscard]] virtual Vec3 getLeftBottomBack() const = 0;
        [[nodiscard]] virtual int compareToOn(const ObjectInterface&, Axis axis) const = 0;

        [[nodiscard]] inline int isOn(Axis axis, double val) const {
            if(val > getRightTopFront()[axis]){
                return 1;
            } else if(val < getLeftBottomBack()[axis]){
                return -1;
            } else {
                return 0;
            }
        }
    };

    class RayInterface{
        friend class KdTree;
    public:
        [[nodiscard]] virtual Vec3 getStartPoint() const = 0;
        [[nodiscard]] virtual Vec3 getDir() const = 0;
    private:
        std::vector<KdNode*> intersect(const KdNode *node) const;
    };


private:

    constexpr static const double PartitionResolution = .5;
    constexpr static const int PartitionThreshold = 10;

    struct KdNode{
        // TopLeftFront, BottomRightBack
        Vec3 LBF, RTB;
        ObjectInterface::Axis axis;
        double val;
        KdNode *leftChild, *rightChild;
        std::vector<const ObjectInterface* > triPtrList;

        ~KdNode();

    };

    struct PartitionResult{
        ObjectInterface::Axis axis;
        double val;
    };

    std::unique_ptr<KdNode> root{nullptr};
    static KdNode *buildTree(std::vector<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF);
    static PartitionResult makePartition(std::vector<const ObjectInterface *> &obj, const Vec3 &LBB, const Vec3 &RTF);

public:

    explicit KdTree(const std::vector<ObjectInterface>&);

    const ObjectInterface* traverse(const RayInterface&) ;

};



#endif //INC_580_FINAL_PROJ_KDTREE_H
