//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_KDTREE_H
#define INC_580_FINAL_PROJ_KDTREE_H

#include "../linear/Vec.h"
#include <vector>
#include <tuple>
#include <algorithm>
#include <concepts>
#include <list>
#include <unordered_set>


class KdTree
{

private:
    struct KdNode;

public:
    struct ObjectInterface{
        enum Axis{
            X = 0, Y = 1, Z = 2, NO = -1
        };
        [[nodiscard]] virtual const Vec3 & getRightTopFront() const = 0;
        [[nodiscard]] virtual const Vec3 & getLeftBottomBack() const = 0;

        [[nodiscard]] inline int isOn(Axis axis, double val) const {
            if(val > getRightTopFront()[axis]){
                return -1;
            } else if(val < getLeftBottomBack()[axis]){
                return 1;
            } else {
                return 0;
            }
        }
    };

    struct RayInterface{
        Vec3 startPoint, dir;

        RayInterface(Vec3 startPoint, Vec3 dir);

        [[nodiscard]] const Vec3& getStartPoint() const ;
        [[nodiscard]] const Vec3& getDir() const ;
        [[nodiscard]] virtual double intersect(const ObjectInterface*) const = 0;
    };

private:
    struct KdNode{
        // TopLeftBack, BottomRightFront
//        Vec3 LBB, RTF;
        ObjectInterface::Axis axis;
        double val;
        KdNode *leftChild, *rightChild;
        std::vector<const ObjectInterface* > triPtrList;

        [[nodiscard]] inline bool hasPartition() const {
            return axis != ObjectInterface::Axis::NO;
        }
        [[nodiscard]] std::vector<KdNode*> intersectingChildren(const RayInterface& ray) const;
        ~KdNode();
        void output(std::ostream &os, std::string* buff = nullptr) const;

    };

    constexpr static const double PartitionResolution = 2;
    constexpr static const int PartitionThreshold = 10;


    struct PartitionResult{
        ObjectInterface::Axis axis;
        double val;
    };

    std::unique_ptr<KdNode> root{nullptr};
    KdNode *buildTree(std::list<const ObjectInterface *> &objs, const Vec3 &LBB, const Vec3 &RTF, int curLayer);
    PartitionResult makePartition(std::list<const ObjectInterface *> &obj, const Vec3 &LBB, const Vec3 &RTF);

    bool traverse(const RayInterface&,
                  const KdTree::KdNode *curNode,
                  ObjectInterface const* &ret,
                  double &curT,
                  std::unordered_set<ObjectInterface const*> &seen) const;

public:


    KdTree() = default;

    template<class T>
    requires std::derived_from<T, ObjectInterface>
    explicit KdTree(const std::vector<T>& oriObjs){
        std::list<const ObjectInterface*> objs;
        Vec3 RTF{INT_MIN, INT_MIN, INT_MIN}, LBB{INT_MAX, INT_MAX, INT_MAX};
        for(const auto &obj : oriObjs){
            objs.emplace_back(static_cast<const ObjectInterface*>(&obj));
            for(auto i : {0, 1, 2}){
                RTF[i] = std::max(RTF[i], obj.getRightTopFront()[i]);
                LBB[i] = std::min(LBB[i], obj.getLeftBottomBack()[i]);
            }
        }

        root = std::move(std::unique_ptr<KdNode>(buildTree(objs, LBB, RTF, 0)));
    };

    template<class T>
    requires std::derived_from<T, ObjectInterface>
    explicit KdTree(const std::vector<const T* >& oriObjs){
        std::list<const ObjectInterface*> objs;
        Vec3 RTF{INT_MIN, INT_MIN, INT_MIN}, LBB{INT_MAX, INT_MAX, INT_MAX};
        for(const auto obj : oriObjs){
            objs.emplace_back(static_cast<const ObjectInterface*>(obj));
            for(auto i : {0, 1, 2}){
                RTF[i] = std::max(RTF[i], obj->getRightTopFront()[i]);
                LBB[i] = std::min(LBB[i], obj->getLeftBottomBack()[i]);
            }
        }

        root = std::move(std::unique_ptr<KdNode>(buildTree(oriObjs, LBB, RTF, 0)));
    };

    [[nodiscard]] const ObjectInterface* traverse(const RayInterface&) const;

    [[nodiscard]] const KdNode *getRoot() const;

    friend std::ostream &operator<<(std::ostream& os, const KdTree&);

public:
    int layer = 0;
    int maxTri = 0;
    int minTri = INT_MAX;
};



#endif //INC_580_FINAL_PROJ_KDTREE_H
