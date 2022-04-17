//
// Created by Jerry Ye on 2022/3/30.
//

#include "KdTree.h"

using namespace std;

KdTree::KdNode* KdTree::buildTree(std::vector<ObjectInterface* const> objs, const Vec3 &RTB, const Vec3 &LBF)
{
    /*
     * 1. Get RTB, LBF
     * 1. Find partition
     * 2. Record
     */

    auto result = makePartition(objs, RTB, LBF);
    Vec3 lRTB{RTB}, lLBF{LBF}, rRTB{RTB}, rLBF{LBF};
    lRTB[result.axis] = result.val;
    rLBF[result.axis] = result.val;

    auto *node = new KdNode{
            RTB, LBF,
            result.axis, result.val,
            result.axis != ObjectInterface::Axis::NO ? buildTree(result.l, lRTB, lLBF) : nullptr,
            result.axis != ObjectInterface::Axis::NO ? buildTree(result.r, rRTB, rLBF) : nullptr,
            move(objs)
    };

    return node;
}

KdTree::PartitionResult
KdTree::makePartition(vector<ObjectInterface *const> &obj, const Vec3 &RTB, const Vec3 &LBF)
{

    return {};
}

