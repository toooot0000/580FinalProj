//
// Created by Jerry Ye on 2022/3/30.
//

#include "KdTree.h"
#include <algorithm>
#include <queue>
#include <list>
#include <utility>

using namespace std;


KdTree::KdNode * KdTree::buildTree(std::vector<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF)
{
    /*
     * 1. Get RTF, LBB
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
                rObjs.splice(rObjs.end(), shrObjs);
            } else if (lObjs.size() < rObjs.size()){
                l = mid;
                targetObjs = &rObjs;
                lObjs.splice(lObjs.end(), shrObjs);
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
        } else if(!cur->hasPartition()){
            auto end = candidates.end();
            candidates.resize(candidates.size() + cur->triPtrList.size());
            copy(cur->triPtrList.begin(), cur->triPtrList.end(), end);
        } else {
            for(const auto next : cur->intersectingChildren(ray)){
                que.emplace(next);
            }
        }
    }
    KdTree::ObjectInterface const* ret;
    double curT = INT_MAX;
    for(auto obj : candidates){
        double t = ray.intersect(obj);
        if(t < curT){
            ret = obj;
        }
    }
    return ret;
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
    auto t = (val - sp)/d;
    if(t>0){
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
    os << *buff << "lbb: " << LBB << "\n";
    os << *buff << "rtf: " << RTF << "\n";

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

