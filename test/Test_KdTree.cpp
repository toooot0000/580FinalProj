//
// Created by Jerry Ye on 2022/4/20.
//

#include "KdTree.h"
#include <catch2/catch_all.hpp>
#include <utility>
#include <vector>


struct TestTri : KdTree::ObjectInterface{
    Vec3 lbb, rtf;
    TestTri(Vec3 lbb, Vec3 rtf) : rtf(std::move(rtf)), lbb(std::move(lbb)) {}
    [[nodiscard]] Vec3 getRightTopFront() const override{
        return rtf;
    };
    [[nodiscard]] Vec3 getLeftBottomBack() const override{
        return lbb;
    };
};



TEST_CASE("Test_KdTree", "[KdTree]"){

    std::vector<TestTri> vec{
        TestTri{
            Vec3{1, 2, 1}, Vec3{2, 2, 2}
        }, TestTri{
            Vec3{2, 3, 2}, Vec3{3, 4, 3}
        }, TestTri{
            Vec3{1, 1, 0}, Vec3{2, 2, 1}
        }
    };

    SECTION("Constructor"){
        KdTree kdTree(vec);
        REQUIRE( kdTree.getRoot() != nullptr );
        SECTION("The root node contains all objects"){
            REQUIRE( kdTree.getRoot()->triPtrList.size() == 3 );
        }

        SECTION("The root node has a left child"){
            REQUIRE( kdTree.getRoot()->leftChild != nullptr );
            INFO("leftChild list size: " << kdTree.getRoot()->leftChild->triPtrList.size() );
        }

        SECTION("The root node has a right child"){
            REQUIRE(kdTree.getRoot()->rightChild != nullptr );
            INFO("rightChild list size: " << kdTree.getRoot()->rightChild->triPtrList.size() );
        }

    }

}
