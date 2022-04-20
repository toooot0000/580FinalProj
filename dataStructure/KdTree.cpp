//
// Created by Jerry Ye on 2022/3/30.
//

#include "KdTree.h"
#include <algorithm>
#include <queue>

using namespace std;


KdTree::KdNode * KdTree::buildTree(std::vector<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF)
{
    /*
     * 1. Get RTB, LBF
     * 1. Find partition
     * 2. Record
     */

    auto result = makePartition(objs, LBB, RTF);
    Vec3 lRTF{RTF}, lLBB{LBB}, rRTF{RTF}, rLBB{LBB};
    lRTF[result.axis] = result.val;
    rLBB[result.axis] = result.val;

    vector<const ObjectInterface *> lObjs, rObjs;

    for(const auto obj: objs){
        switch (obj->isOn(result.axis, result.val)){
            case 1:
                rObjs.emplace_back(obj);
                break;
            case -1:
                lObjs.emplace_back(obj);
                break;
            default:
                rObjs.emplace_back(obj);
                lObjs.emplace_back(obj);
                break;
        }
    }

    auto *node = new KdNode{
            LBB, RTF,
            result.axis, result.val,
            result.axis != ObjectInterface::Axis::NO ? buildTree(lObjs, lLBB, lRTF) : nullptr,
            result.axis != ObjectInterface::Axis::NO ? buildTree(rObjs, rLBB, rRTF) : nullptr,
            move(objs)
    };

    return node;
}

KdTree::PartitionResult KdTree::makePartition(std::vector<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF)
{
    auto resAxis = ObjectInterface::Axis::NO;
    double resVal = INT_MAX;

    size_t form = INT_MAX, bestForm = INT_MAX;

    for(auto curAxis : {ObjectInterface::Axis::X, ObjectInterface::Axis::Y, ObjectInterface::Axis::Z}){
        vector<const ObjectInterface*> lObjs, rObjs, shrObjs;
        vector<const ObjectInterface*> & targetObjs = objs;
        auto l = LBB[curAxis], r = RTF[curAxis], mid = .0;

        while((r - l) > PartitionResolution && form > PartitionThreshold){
            mid = (r-l)/2 + l;
            for(const auto obj : targetObjs){
                switch (obj->isOn(curAxis, mid)){
                    case 0:
                        shrObjs.emplace_back(obj);
                        break;
                    case 1:
                        rObjs.emplace_back(obj);
                        break;
                    case -1:
                        lObjs.emplace_back(obj);
                        break;
                    default:
                        break;
                }
            }
            form = std::abs((long)lObjs.size() - (long)rObjs.size()) + shrObjs.size();
            if (form < bestForm){
                bestForm = form;
                resVal = mid;
                resAxis = curAxis;
            }
            if (form == objs.size()){
                break;
            }
            if(lObjs.size() > rObjs.size()){
                r = mid;
                targetObjs = lObjs;
                for(const auto obj : shrObjs){
                    rObjs.emplace_back(obj);
                }
                shrObjs.clear();
            } else if (lObjs.size() < rObjs.size()){
                l = mid;
                targetObjs = rObjs;
                for(const auto obj : shrObjs){
                    lObjs.emplace_back(obj);
                }
                shrObjs.clear();
            } else {
                break;
            }
        }
    }
    if(form == objs.size()){
        resAxis = ObjectInterface::Axis::NO;
    }
    return {resAxis, resVal};
}

KdTree::KdTree(const vector<ObjectInterface> &oriObjs)
{
    vector<const ObjectInterface*> objs;
    Vec3 RTF, LBB;
    for(const auto &obj : oriObjs){
        objs.emplace_back(&obj);
        for(auto i : {0, 1, 2}){
            RTF[i] = max(RTF[i], obj.getRightTopFront()[i]);
            LBB[i] = min(LBB[i], obj.getLeftBottomBack()[i]);
        }
    }

    root = move(unique_ptr<KdNode>(buildTree(objs, LBB, RTF)));
}

const KdTree::ObjectInterface *KdTree::traverse(const KdTree::RayInterface &ray)
{
    queue<KdNode* > que;
    que.emplace(root.get());
    vector<const ObjectInterface*> candidates;
    while(!que.empty()){
        auto cur = que.front();
        que.pop();
        if(!cur){
            continue;
        } else if(!cur->rightChild && !cur->leftChild){
            copy(cur->triPtrList.begin(), cur->triPtrList.end(), candidates.end());
        } else {
            for(const auto next : ray.intersect(cur)){
                que.emplace(next);
            }
        }
    }

    return nullptr;
}

vector<KdTree::KdNode* > KdTree::RayInterface::intersect(const KdTree::KdNode *node) const
{
    if(!node){
        return {};
    }
    if(node->axis == ObjectInterface::Axis::NO){
        return {};
    }
    auto sp = getStartPoint()[node->axis];
    auto d = getDir()[node->axis];
    auto t = (node->val - sp)/d;
    if(t>0){
        if(sp < node->val){
            return {node->leftChild, node->rightChild};
        } else {
            return {node->rightChild, node->leftChild};
        }
    } else {
        if(sp < node->val){
            return {node->leftChild};
        } else {
            return {node->rightChild};
        }
    }
}

KdTree::KdNode::~KdNode()
{
    delete leftChild;
    delete rightChild;
}
