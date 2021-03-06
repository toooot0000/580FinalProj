//
// Created by Jerry Ye on 2022/3/30.
//

#include "KdTree.h"
#include <algorithm>
#include <queue>
#include <list>
#include <utility>
#include <iostream>

using namespace std;


KdTree::KdNode * KdTree::buildTree(std::list<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF, int curLayer)
{
    /*
     * 1. Get RTF, LBB
     * 1. Find partition
     * 2. Record
     */
    if(objs.size()<PartitionThreshold){
        return nullptr;
    }
    auto result = makePartition(objs, LBB, RTF);
    Vec3 lRTF{RTF}, lLBB{LBB}, rRTF{RTF}, rLBB{LBB};
    lRTF[result.axis] = result.val;
    rLBB[result.axis] = result.val;

    list<const ObjectInterface *> lObjs, rObjs;

    if(result.axis != ObjectInterface::Axis::NO){
        auto it = objs.begin();
        decltype(it) prev;
        while(it != objs.end()){
            prev = it;
            auto &obj = *it;
            ++it;
            switch (obj->isOn(result.axis, result.val)){
                case 1:
                    rObjs.splice(rObjs.end(), objs, prev, next(prev));
                    break;
                case -1:
                    lObjs.splice(lObjs.end(), objs, prev, next(prev));
                    break;
                default:
                    rObjs.push_back(obj);
                    lObjs.splice(lObjs.end(), objs, prev, next(prev));
                    break;
            }
        }
    }

    auto *node = new KdNode{
//            LBB, RTF,
            result.axis, result.val,
            result.axis != ObjectInterface::Axis::NO ? buildTree(lObjs, lLBB, lRTF, curLayer+1) : nullptr,
            result.axis != ObjectInterface::Axis::NO ? buildTree(rObjs, rLBB, rRTF, curLayer+1) : nullptr,
            {objs.begin(), objs.end()}
    };
    if(node->triPtrList.size() > maxTri){
        maxTri = node->triPtrList.size();
    }
    if(result.axis == ObjectInterface::Axis::NO && node->triPtrList.size() < minTri){
        minTri = node->triPtrList.size();
    }
    if(result.axis == ObjectInterface::Axis::NO && curLayer > layer){
        layer = curLayer;
    }
    return node;
}

KdTree::PartitionResult KdTree::makePartition(std::list<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF)
{
    auto resAxis = ObjectInterface::Axis::NO;
    double resVal = INT_MAX;

    size_t form = INT_MAX, bestForm = INT_MAX;

    for(auto curAxis : {ObjectInterface::Axis::X, ObjectInterface::Axis::Y, ObjectInterface::Axis::Z}){
        list<const ObjectInterface*> lObjs(objs.begin(), objs.end()), rObjs, shrObjs;
        list<const ObjectInterface*> *targetObjs = &lObjs;
        auto l = LBB[curAxis], r = RTF[curAxis], mid = .0;

        while((r - l) > PartitionResolution && form > PartitionThreshold){
            mid = (r-l)/2 + l;
            auto it = targetObjs->begin();
            decltype(it) prev;
            while(it != targetObjs->end()){
                auto obj = *it;
                prev = it;
                ++it;
                switch (obj->isOn(curAxis, mid)){
                    case 0:
                        shrObjs.splice(shrObjs.end(), *targetObjs, prev, std::next(prev));
                        break;
                    case 1:
                        if(targetObjs != &rObjs){
                            rObjs.splice(rObjs.end(), *targetObjs, prev, std::next(prev));
                        }
                        break;
                    case -1:
                        if(targetObjs != &lObjs){
                            lObjs.splice(lObjs.end(), *targetObjs, prev, std::next(prev));
                        }
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
                targetObjs = &lObjs;
                lObjs.splice(rObjs.end(), shrObjs);
            } else if (lObjs.size() < rObjs.size()){
                l = mid;
                targetObjs = &rObjs;
                rObjs.splice(lObjs.end(), shrObjs);
            } else {
                break;
            }
        }
    }
    if(bestForm == objs.size()){
        resAxis = ObjectInterface::Axis::NO;
    }
    return {resAxis, resVal};
}

const KdTree::KdNode *KdTree::getRoot() const
{
    return static_cast<const KdNode*>(root.get());
}

std::ostream &operator<<(std::ostream& os, const KdTree &tree)
{
    tree.getRoot()->output(os);
    return os;
}

bool KdTree::traverse(
        const KdTree::RayInterface &ray,
        const KdTree::KdNode *curNode,
        ObjectInterface const* &res,
        double &curT,
        std::unordered_set<ObjectInterface const*> &seen
        ) const
{
    if(!curNode){
        return false;
    }
    if(!curNode->hasPartition()){
        bool ret = false;
        for(auto& obj : curNode->triPtrList){
            if(seen.contains(obj)){
                continue;
            }
            seen.emplace(obj);
            auto t = ray.intersect(obj);
            if( t > 0 && ( res == nullptr || t < curT )){
                res = obj;
                curT = t;
                ret = true;
            }
        }
        return ret;
    } else {
        for(const auto& nextNode : curNode->intersectingChildren(ray)){
            if(traverse(ray, nextNode, res, curT, seen)){
                return true;
            }
        }
        return false;
    }
}


const KdTree::ObjectInterface* KdTree::traverse(const KdTree::RayInterface& ray) const{

    unordered_set<ObjectInterface const*> seen;
    ObjectInterface const* ret = nullptr;
    double t = -INT_MAX;
    traverse(ray, root.get(), ret, t, seen);
    return ret;
}


KdTree::KdNode::~KdNode()
{
    delete leftChild;
    delete rightChild;
}

std::vector<KdTree::KdNode*> KdTree::KdNode::intersectingChildren(const KdTree::RayInterface& ray) const{
    if(axis == ObjectInterface::Axis::NO){
        return {};
    }
    auto sp = ray.getStartPoint()[axis];
    auto d = ray.getDir()[axis];

    if((val - sp) * d >= 0){
        if(sp < val){
            return {leftChild, rightChild};
        } else {
            return {rightChild, leftChild};
        }
    } else {
        if(sp < val){
            return {leftChild};
        } else {
            return {rightChild};
        }
    }
}

const Vec3 &KdTree::RayInterface::getStartPoint() const
{
    return startPoint;
}

const Vec3 &KdTree::RayInterface::getDir() const
{
    return dir;
}

KdTree::RayInterface::RayInterface(Vec3 startPoint, Vec3 dir) : startPoint(std::move(startPoint)), dir(std::move(dir))
{}


void KdTree::KdNode::output(ostream &os, std::string* buff) const {
    if(!buff){
        buff = new std::string();
    }
    os << *buff << "[\n";
    os << *buff << "axis: " << axis << "\n";
    os << *buff << "val: " << val << "\n";

    buff->resize(buff->size()+2, '-');

    if(leftChild){
        leftChild->output(os, buff);
    }
    if(rightChild){
        rightChild->output(os, buff);
    }

    buff->resize(buff->size() - 2);

    os << *buff << "]\n";

    if(buff->empty()){
        delete buff;
    }
}

