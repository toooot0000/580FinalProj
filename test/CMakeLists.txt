cmake_minimum_required(VERSION 3.20)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

add_executable(Test_KdTree Test_KdTree.cpp)
if(NOT TARGET 580_Final_Proj)
    add_subdirectory(../dataStructure)
endif()
target_link_libraries(Test_KdTree PUBLIC dataStruct)
target_link_libraries(Test_KdTree PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(Test_KdTree)