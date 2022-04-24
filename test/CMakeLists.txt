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


add_executable(Test_RayCast Test_RayCast.cpp)
if(NOT TARGET 580_Final_Proj)
    add_subdirectory(../renderer)
endif()
target_link_libraries(Test_RayCast PUBLIC renderer)
target_link_libraries(Test_RayCast PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(Test_RayCast)