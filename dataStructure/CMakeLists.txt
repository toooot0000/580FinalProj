cmake_minimum_required(VERSION 3.2)

add_library(dataStruct BSP.h BSP.cpp KdTree.cpp KdTree.h)
set_target_properties(dataStruct PROPERTIES CXX_STANDARD 20)
target_include_directories(dataStruct PUBLIC ./)
